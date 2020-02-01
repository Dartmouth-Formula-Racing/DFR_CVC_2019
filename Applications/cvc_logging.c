/*
 * cvc_logging.c
 *
 *  Created on: Feb 6, 2019
 *      Author: f002bc7
 */

/* Includes ------------------------------------------------------------------*/

#include "cvc_logging.h"

/* Global Variables ------------------------------------------------------------------*/

uint8_t LOG_FLAG = 0;

/* Private Global Variables ------------------------------------------------------------------*/

static uint32_t buff[3000U];
static FATFS SD_FatFs; /* File system object for User logical drive */
static FIL LogFile; /* File object */
static FIL CopyFile;
static char SDPath[4]; /* SD disk logical drive path */
static uint32_t wbytes; /* File write counts */
static uint32_t rbytes; /* File read counts */

static uint8_t n_outputs;	/* number of outputs to log */
static uint32_t n_writes = 0;	/* number of writes since power cycle */

//TODO: add logging for pm100s
/* CAN inputs to log */
CAN_input_t CAN_logging[] =
{

		MODULE_A_TEMP,
			MODULE_B_TEMP,
			MODULE_C_TEMP,
			GATE_DRIVER_BOARD_TEMP,
			CONTROL_BOARD_TEMPERATURE,
			RTD_1_TEMP,
			RTD_2_TEMP,
			RTD_3_TEMP,
			RTD_4_TEMP,
			RTD_5_TEMP,
			MOTOR_TEMP,
			TORQUE_SHUDDER,
			ANALOG_INPUT_1,
			ANALOG_INPUT_2,
			ANALOG_INPUT_3,
			ANALOG_INPUT_4,
			ANALOG_INPUT_5,
			ANALOG_INPUT_6,
			DIGITAL_INPUT_1,
			DIGITAL_INPUT_2,
			DIGITAL_INPUT_3,
			DIGITAL_INPUT_4,
			DIGITAL_INPUT_5,
			DIGITAL_INPUT_6,
			DIGITAL_INPUT_7,
			DIGITAL_INPUT_8,
			MOTOR_ANGLE_ELECTRICAL,
			MOTOR_SPEED,
			ELECTRICAL_OUTPUT_FREQ,
			DELTA_RESOLVER_FILTERED,
			PHASE_A_CURRENT,
			PHASE_B_CURRENT,
			PHASE_C_CURRENT,
			DC_BUS_CURRENT,
			DC_BUS_VOLTAGE,
			OUTPUT_VOLTAGE,
			VAB_VD_VOLTAGE,
			VBC_VQ_VOLTAGE,
			FLUX_COMMAND,
			FLUX_FEEDBACK,
			ID_FEEDBACK,
			IQ_FEEDBACK,
			REFERENCE_VOLTAGE_1ANDHALFV,
			REFERENCE_VOLTAGE_2ANDHALFV,
			REFERENCE_VOLTAGE_5V,
			REFERENCE_VOLTAGE_12V,
			VSM_STATE,
			INVERTER_STATE,
			RELAY_STATE,
			INVERTER_RUN_MODE,
			INVERTER_ACTIVE_DISCHARGE_STATE,
			INVERTER_COMMAND_MODE,
			INVERTER_ENABLE_STATE,
			INVERTER_ENABLE_LOCKOUT,
			DIRECTION_COMMAND,
			BMS_ACTIVE,
			BMS_LIMITING_TORQUE,
			POST_FAULT_LO,
			POST_FAULT_HI,
			RUN_FAULT_LO,
			RUN_FAULT_HI,
			COMMANDED_TORQUE,
			TORQUE_FEEDBACK,
			POWER_ON_TIMER,
			MODULATION_INDEX,
			FLUX_WEAKENING_OUTPUT,
			ID_COMMAND,
			IQ_COMMAND,
			EEPROM_VERSION, //EEPROM Version / Project Code
			SOFTWARE_VERSION,
			DATE_CODE_MD,
			DATE_CODE_YY,
			DIAGNOSTIC_DATA_LO,
			DIAGNOSTIC_DATA_HI,
			PARAMETER_RESPONSE_ADDRESS, //address stores the address of the parameter and the success or not byte
			PARAMETER_RESPONSE_DATA, //update the OFFSET_END macro when changing the var at this location

			//second drive inverter, pm-100, Rinehart
			MODULE_A_TEMP_2,
			MODULE_B_TEMP_2,
			MODULE_C_TEMP_2,
			GATE_DRIVER_BOARD_TEMP_2,
			CONTROL_BOARD_TEMPERATURE_2,
			RTD_1_TEMP_2,
			RTD_2_TEMP_2,
			RTD_3_TEMP_2,
			RTD_4_TEMP_2,
			RTD_5_TEMP_2,
			MOTOR_TEMP_2,
			TORQUE_SHUDDER_2,
			ANALOG_INPUT_1_2,
			ANALOG_INPUT_2_2,
			ANALOG_INPUT_3_2,
			ANALOG_INPUT_4_2,
			ANALOG_INPUT_5_2,
			ANALOG_INPUT_6_2,
			DIGITAL_INPUT_1_2,
			DIGITAL_INPUT_2_2,
			DIGITAL_INPUT_3_2,
			DIGITAL_INPUT_4_2,
			DIGITAL_INPUT_5_2,
			DIGITAL_INPUT_6_2,
			DIGITAL_INPUT_7_2,
			DIGITAL_INPUT_8_2,
			MOTOR_ANGLE_ELECTRICAL_2,
			MOTOR_SPEED_2,
			ELECTRICAL_OUTPUT_FREQ_2,
			DELTA_RESOLVER_FILTERED_2,
			PHASE_A_CURRENT_2,
			PHASE_B_CURRENT_2,
			PHASE_C_CURRENT_2,
			DC_BUS_CURRENT_2,
			DC_BUS_VOLTAGE_2,
			OUTPUT_VOLTAGE_2,
			VAB_VD_VOLTAGE_2,
			VBC_VQ_VOLTAGE_2,
			FLUX_COMMAND_2,
			FLUX_FEEDBACK_2,
			ID_FEEDBACK_2,
			IQ_FEEDBACK_2,
			REFERENCE_VOLTAGE_1ANDHALFV_2,
			REFERENCE_VOLTAGE_2ANDHALFV_2,
			REFERENCE_VOLTAGE_5V_2,
			REFERENCE_VOLTAGE_12V_2,
			VSM_STATE_2,
			INVERTER_STATE_2,
			RELAY_STATE_2,
			INVERTER_RUN_MODE_2,
			INVERTER_ACTIVE_DISCHARGE_STATE_2,
			INVERTER_COMMAND_MODE_2,
			INVERTER_ENABLE_STATE_2,
			INVERTER_ENABLE_LOCKOUT_2,
			DIRECTION_COMMAND_2,
			BMS_ACTIVE_2,
			BMS_LIMITING_TORQUE_2,
			POST_FAULT_LO_2,
			POST_FAULT_HI_2,
			RUN_FAULT_LO_2,
			RUN_FAULT_HI_2,
			COMMANDED_TORQUE_2,
			TORQUE_FEEDBACK_2,
			POWER_ON_TIMER_2,
			MODULATION_INDEX_2,
			FLUX_WEAKENING_OUTPUT_2,
			ID_COMMAND_2,
			IQ_COMMAND_2,
			EEPROM_VERSION_2, //EEPROM Version / Project Code
			SOFTWARE_VERSION_2,
			DATE_CODE_MD_2,
			DATE_CODE_YY_2,
			DIAGNOSTIC_DATA_LO_2,
			DIAGNOSTIC_DATA_HI_2,
			PARAMETER_RESPONSE_ADDRESS_2,
			PARAMETER_RESPONSE_DATA_2
};

char * header = "TICKS, MODULE_A_TEMP, MODULE_B_TEMP, MODULE_C_TEMP, GATE_DRIVER_BOARD_TEMP, CONTROL_BOARD_TEMPERATURE, RTD_1_TEMP, RTD_2_TEMP, RTD_3_TEMP, RTD_4_TEMP, RTD_5_TEMP, MOTOR_TEMP, TORQUE_SHUDDER, ANALOG_INPUT_1, ANALOG_INPUT_2, ANALOG_INPUT_3, ANALOG_INPUT_4, ANALOG_INPUT_5, ANALOG_INPUT_6, DIGITAL_INPUT_1, DIGITAL_INPUT_2, DIGITAL_INPUT_3, DIGITAL_INPUT_4, DIGITAL_INPUT_5, DIGITAL_INPUT_6, DIGITAL_INPUT_7, DIGITAL_INPUT_8, MOTOR_ANGLE_ELECTRICAL, MOTOR_SPEED, ELECTRICAL_OUTPUT_FREQ, DELTA_RESOLVER_FILTERED PHASE_A_CURRENT, PHASE_B_CURRENT, PHASE_C_CURRENT, DC_BUS_CURRENT, DC_BUS_VOLTAGE, OUTPUT_VOLTAGE, VAB_VD_VOLTAGE, VBC_VQ_VOLTAGE, FLUX_COMMAND,FLUX_FEEDBACK, ID_FEEDBACK, IQ_FEEDBACK, REFERENCE_VOLTAGE_1ANDHALFV, REFERENCE_VOLTAGE_2ANDHALFV, REFERENCE_VOLTAGE_5V, REFERENCE_VOLTAGE_12V, VSM_STATE, INVERTER_STATE, RELAY_STATE, INVERTER_RUN_MODE, INVERTER_ACTIVE_DISCHARGE_STATE, INVERTER_COMMAND_MODE, INVERTER_ENABLE_STATE, INVERTER_ENABLE_LOCKOUT, DIRECTION_COMMAND, BMS_ACTIVE, BMS_LIMITING_TORQUE, POST_FAULT_LO, POST_FAULT_HI, RUN_FAULT_LO, RUN_FAULT_HI, COMMANDED_TORQUE, TORQUE_FEEDBACK, POWER_ON_TIMER, MODULATION_INDEX, FLUX_WEAKENING_OUTPUT, ID_COMMAND, IQ_COMMAND, EEPROM_VERSION, SOFTWARE_VERSION, DATE_CODE_MD, DATE_CODE_YY, DIAGNOSTIC_DATA_LO, DIAGNOSTIC_DATA_HI, PARAMETER_RESPONSE_ADDRESS, PARAMETER_RESPONSE_DATA, MODULE_A_TEMP_2, MODULE_B_TEMP_2, MODULE_C_TEMP_2, GATE_DRIVER_BOARD_TEMP_2,CONTROL_BOARD_TEMPERATURE_2, RTD_1_TEMP_2, RTD_2_TEMP_2, RTD_3_TEMP_2, RTD_4_TEMP_2, RTD_5_TEMP_2, MOTOR_TEMP_2, TORQUE_SHUDDER_2, ANALOG_INPUT_1_2, ANALOG_INPUT_2_2, ANALOG_INPUT_3_2, ANALOG_INPUT_4_2, ANALOG_INPUT_5_2, ANALOG_INPUT_6_2, DIGITAL_INPUT_1_2, DIGITAL_INPUT_2_2, DIGITAL_INPUT_3_2, DIGITAL_INPUT_4_2, DIGITAL_INPUT_5_2, DIGITAL_INPUT_6_2, DIGITAL_INPUT_7_2, DIGITAL_INPUT_8_2, MOTOR_ANGLE_ELECTRICAL_2, MOTOR_SPEED_2, ELECTRICAL_OUTPUT_FREQ_2, DELTA_RESOLVER_FILTERED_2, PHASE_A_CURRENT_2, PHASE_B_CURRENT_2, PHASE_C_CURRENT_2, DC_BUS_CURRENT_2, DC_BUS_VOLTAGE_2, OUTPUT_VOLTAGE_2, VAB_VD_VOLTAGE_2, VBC_VQ_VOLTAGE_2, FLUX_COMMAND_2, FLUX_FEEDBACK_2, ID_FEEDBACK_2, IQ_FEEDBACK_2, REFERENCE_VOLTAGE_1ANDHALFV_2, REFERENCE_VOLTAGE_2ANDHALFV_2, REFERENCE_VOLTAGE_5V_2, REFERENCE_VOLTAGE_12V_2, VSM_STATE_2, INVERTER_STATE_2, RELAY_STATE_2, INVERTER_RUN_MODE_2, INVERTER_ACTIVE_DISCHARGE_STATE_2, INVERTER_COMMAND_MODE_2, INVERTER_ENABLE_STATE_2, INVERTER_ENABLE_LOCKOUT_2, DIRECTION_COMMAND_2, BMS_ACTIVE_2, BMS_LIMITING_TORQUE_2, POST_FAULT_LO_2, POST_FAULT_HI_2, RUN_FAULT_LO_2, RUN_FAULT_HI_2, COMMANDED_TORQUE_2, TORQUE_FEEDBACK_2, POWER_ON_TIMER_2, MODULATION_INDEX_2, FLUX_WEAKENING_OUTPUT_2, ID_COMMAND_2, IQ_COMMAND_2, EEPROM_VERSION_2, SOFTWARE_VERSION_2, DATE_CODE_MD_2, DATE_CODE_YY_2, DIAGNOSTIC_DATA_LO_2, DIAGNOSTIC_DATA_HI_2, PARAMETER_RESPONSE_ADDRESS_2, PARAMETER_RESPONSE_DATA_2\n";	/* labels for file contents (separate by commas for csv */

/* Functions ------------------------------------------------------------------*/

/**
 * @brief initialize logging functionality
 */
int logging_init()
{
	/* link/initialize SD driver */
	if (FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
	{
		/* mount disk */
		if (f_mount(&SD_FatFs, (char const*) SDPath, 1) != FR_OK)
		{
			return -2;
		}
	}
	else
	{
		return -1;
	}

	/* get number of logging outputs */
	n_outputs = sizeof(CAN_logging) / sizeof(CAN_input_t);

	if (f_open(&LogFile, "LOG_FILE_2.csv", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
	{
		return -3;
	}

	return 1;

}


void log_data(void)
{

	char *pbuff = (char *) buff;

	TickType_t ticks = xTaskGetTickCount();

	uint8_t i = 0;

	/* add header to buffer */
	if (n_writes == 0){

		strcpy(pbuff+i, header);


		i+=strlen(pbuff+i);
	}

	/* add current ticks to buffer */
	utoa(ticks, pbuff+i, 10);
	i+=strlen(pbuff+i);
	*(pbuff+i) = ',';	// insert ASCII code for "," character
	i++;

	/* loop through logging outputs and add to buffer */
	for (uint8_t j = 0; j < n_outputs; j++)
	{

		xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);
		utoa(CAN_inputs[CAN_logging[j]], pbuff+i, 10);

		xSemaphoreGive(CAN_Inputs_Vector_Mutex);

		i+=strlen(pbuff+i);

		if (j<n_outputs-1)
		{
			*(pbuff+i) = ',';	// separate values of same row by commas
		}
		else
		{
			*(pbuff+i) = '\n';		// insert new line at end of row
		}
		i++;
	}



	/* write data to file */
	if(f_write(&LogFile, pbuff, i, (void *)&wbytes) != FR_OK)
	{

		f_close(&LogFile);
		BSP_LED_On(LED_RED);

	}

	/* increase number of writes */
	n_writes++;

	BSP_LED_On(LED_GREEN);


#if LOGGING_TEST

	if (n_writes >= 1000)
	{
		BSP_LED_On(LED_BLUE);
		f_close(&LogFile);
		vTaskSuspend(NULL);
	}
#else

	if (LOG_FLAG == 0)
	{
		BSP_LED_On(LED_BLUE);
		f_close(&LogFile);
		vTaskSuspend(NULL);
	}

#endif	/* LOGGING_TEST */

}

void copy_file(void)
{
	char *pbuff = (char *) buff;

	/* create copy of existing file */
	if (f_open(&LogFile, "STM32.csv", FA_OPEN_EXISTING | FA_READ) == FR_OK && f_open(&CopyFile, "STM32_copy.csv", FA_OPEN_ALWAYS | FA_WRITE) == FR_OK)
	{
		FSIZE_t file_size = f_size(&LogFile);
		int buff_size = sizeof(buff);

		int n = file_size / buff_size;
		int r = file_size % buff_size;

		for (int j = 0; j <= n; j++)
		{

			if(f_read(&LogFile, pbuff, (j < n) ? buff_size : r, (void *)&rbytes) != FR_OK)
			{
				f_close(&LogFile);
				f_close(&CopyFile);
				BSP_LED_On(LED_RED);
			}
			if(f_write(&CopyFile, pbuff, rbytes, (void *)&wbytes) != FR_OK)
			{
				f_close(&LogFile);
				f_close(&CopyFile);
				BSP_LED_On(LED_RED);
			}
		}

		f_close(&LogFile);
		f_close(&CopyFile);

		BSP_LED_On(LED_BLUE);
	}
	else
	{
		BSP_LED_On(LED_RED);
	}
}


