/*
 * cvc_can.c
 *
 *  Created on: Nov 30, 2018
 *      Author: f002ccx
 */

/* Includes ------------------------------------------------------------------------*/
#include "cvc_can.h"
#include "FreeRTOS.h"
#include "task.h"
#include "demo.h"
#include "queue.h"
#include "semphr.h"
#include "pm100.h"



/* Private Function Prototypes ---------------------------------------------------------------*/
static void CAN_Config(void);
static void CAN_parser_std(queue_msg_t q_msg, uint8_t CAN_idx);
static void CAN_parser_std_Rinehart(queue_msg_t q_msg, uint8_t CAN_idx);
static void CAN_parser_EMUS1(queue_msg_t q_msg, uint8_t CAN_idx);
static void CAN_parser_ANALOGVOLT(queue_msg_t q_msg, uint8_t CAN_idx);
static void CAN_parser_INTST(queue_msg_t q_msg, uint8_t CAN_idx);
static void CAN_parser_DIAGNOSTIC(queue_msg_t q_msg, uint8_t CAN_idx);

/* Private Variables ---------------------------------------------------------------*/
CAN_HandleTypeDef		CanHandle;
uint32_t 				TxMailbox;


/* CAN message input maps */
/* INPUT_INDEX, START BYTE, START BIT, BYTE SIZE */


input_map_t EMUS2_map[] =
{
		{MIN_CELL_TEMP, 0, 0, 1},
		{MAX_CELL_TEMP, 1, 0, 1},
		{AVG_CELL_TEMP, 2, 0, 1},
};

input_map_t EMUS3_map[] =
{
		{BATT_CURRENT, 0, 0, 2},
		{BATT_CHARGE, 2, 0, 2},
		{BATT_SOC, 6, 0, 1},
};

//Rinehart pm-100 maps

input_map_t TEMP1_map[] =
{
		{MODULE_A_TEMP, 0, 0 ,2},
		{MODULE_B_TEMP, 2, 0,2},
		{MODULE_C_TEMP, 4, 0,2},
		{GATE_DRIVER_BOARD_TEMP, 6, 0,2},
};

input_map_t TEMP2_map[] =
{
		{CONTROL_BOARD_TEMPERATURE, 0, 0 ,2},
		{RTD_1_TEMP, 2, 0,2},
		{RTD_2_TEMP, 4, 0,2},
		{RTD_3_TEMP, 6, 0,2},
};

input_map_t TEMP3_map[] =
{
		{RTD_4_TEMP, 0, 0 ,2},
		{RTD_5_TEMP, 2, 0,2},
		{MOTOR_TEMP, 4, 0,2},
		{TORQUE_SHUDDER, 6, 0,2},
};

input_map_t DIGI_map[] =
{
		{DIGITAL_INPUT_1, 0, 0 ,1},
		{DIGITAL_INPUT_2, 1, 0 ,1},
		{DIGITAL_INPUT_3, 2, 0 ,1},
		{DIGITAL_INPUT_4, 3, 0 ,1},
		{DIGITAL_INPUT_5, 4, 0 ,1},
		{DIGITAL_INPUT_6, 5, 0 ,1},
		{DIGITAL_INPUT_7, 6, 0 ,1},
		{DIGITAL_INPUT_8, 7, 0 ,1},
};

input_map_t MOTORPOS_map[] =
{
		{MOTOR_ANGLE_ELECTRICAL, 0, 0 ,2},
		{MOTOR_SPEED, 2, 0,2},
		{ELECTRICAL_OUTPUT_FREQ, 4, 0,2},
		{DELTA_RESOLVER_FILTERED, 6, 0,2},
};

input_map_t CURRENTINF_map[] =
{

		{PHASE_A_CURRENT, 0, 0, 2},
		{PHASE_B_CURRENT, 2, 0, 2},
		{PHASE_C_CURRENT, 4, 0, 2},
		{DC_BUS_CURRENT, 6, 0, 2},
};

input_map_t VOLTINF_map[] =
{
		{DC_BUS_VOLTAGE, 0, 0, 2},
		{OUTPUT_VOLTAGE, 2, 0, 2},
		{VAB_VD_VOLTAGE, 4, 0, 2},
		{VBC_VQ_VOLTAGE, 6, 0, 2},
};

input_map_t FLUXINF_map[] =
{
		{FLUX_COMMAND, 0, 0 ,2},
		{FLUX_FEEDBACK, 2, 0,2},
		{ID_FEEDBACK, 4, 0,2},
		{IQ_FEEDBACK, 6, 0,2},
};

input_map_t INTVOLT_map[] =
{

		{REFERENCE_VOLTAGE_1ANDHALFV, 0, 0 ,2},
		{REFERENCE_VOLTAGE_2ANDHALFV, 2, 0,2},
		{REFERENCE_VOLTAGE_5V, 4, 0,2},
		{REFERENCE_VOLTAGE_12V, 6, 0,2},

};

input_map_t FAULTCODES_map[] =
{

		{POST_FAULT_LO, 0, 0 ,2},
		{POST_FAULT_HI, 2, 0,2},
		{RUN_FAULT_LO, 4, 0,2},
		{RUN_FAULT_HI, 6, 0,2},
};

input_map_t TORQTIM_map[] =
{
		{COMMANDED_TORQUE, 0, 0, 2},
		{TORQUE_FEEDBACK, 2, 0, 2},
		{POWER_ON_TIMER, 4, 0, 8},
};

input_map_t MODFLUX_map[] =
{

		{MODULATION_INDEX, 0, 0 ,2},
		{FLUX_WEAKENING_OUTPUT, 2, 0,2},
		{ID_COMMAND, 4, 0,2},
		{IQ_COMMAND, 6, 0,2},
};

input_map_t FIRMINF_map[] =
{

		{EEPROM_VERSION, 0, 0 ,2},
		{SOFTWARE_VERSION, 2, 0,2},
		{DATE_CODE_MD, 4, 0,2},
		{DATE_CODE_YY, 6, 0,2},
};

input_map_t PARAMETER_RESPONSE_map[] =
{

		{PARAMETER_RESPONSE_ADDRESS, 0, 0 ,3},
		{PARAMETER_RESPONSE_DATA, 4, 0,2},

};
/* CAN message dictionary */
static CAN_msg_t CAN_dict[]	=
{


		// EMUS BMS messages
		{0x1B6, STD, 0, "EMUS1", NULL, 0, CAN_parser_EMUS1}, 		// EMUS BMS 1 (3)
		{0x1B7, STD, 0, "EMUS2", EMUS2_map, 3, CAN_parser_std}, 		// EMSU BMS 2 (4)
		{0x1BA, STD, 0, "EMUS3", EMUS3_map, 3, CAN_parser_std}, 		// EMSU BMS 3 (5)


		//Rinehart pm-100 messages #1
		{CAN_ID_OFFSET1+0x00, STD, 0, "1_Temp_1", TEMP1_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET1+0x01, STD, 0, "1_Temp_2", TEMP2_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET1+0x02, STD, 0, "1_Temp_3_Torque_Shudder", TEMP3_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET1+0x03, STD, 0, "1_Analog_Input_Volt", NULL, 0, CAN_parser_ANALOGVOLT},
		{CAN_ID_OFFSET1+0x04, STD, 0, "1_Digital_Input_Stat", DIGI_map, 8, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET1+0x05, STD, 0, "1_Motor_Position_Info", MOTORPOS_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET1+0x06, STD, 0, "1_Current_Info", CURRENTINF_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET1+0x07, STD, 0, "1_Voltage_Info", VOLTINF_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET1+0x08, STD, 0, "1_Flux_Info", FLUXINF_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET1+0x09, STD, 0, "1_Internal_Volt", INTVOLT_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET1+0x0A, STD, 0, "1_Internal_States", NULL, 0, CAN_parser_INTST},
		{CAN_ID_OFFSET1+0x0B, STD, 0, "1_Fault_Codes", FAULTCODES_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET1+0x0C, STD, 0, "1_Torque_Timer_Info", TORQTIM_map, 3, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET1+0x0D, STD, 0, "1_Mod_Indx_FluxWeak", MODFLUX_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET1+0x0E, STD, 0, "1_Firm_Info", FIRMINF_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET1+0x0F, STD, 0, "1_Diagnostic", NULL, 0, CAN_parser_DIAGNOSTIC},
		{CAN_ID_OFFSET1+0x22, STD, 0, "1_Parameter_Response", PARAMETER_RESPONSE_map, 0, CAN_parser_std_Rinehart},

		//Rinehart pm-100 messages #2
		{CAN_ID_OFFSET2+0x00, STD, 0, "2_Temp_1", TEMP1_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET2+0x01, STD, 0, "2_Temp_2", TEMP2_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET2+0x02, STD, 0, "2_Temp_3_Torque_Shudder", TEMP3_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET2+0x03, STD, 0, "2_Analog_Input_Volt", NULL, 0, CAN_parser_ANALOGVOLT},
		{CAN_ID_OFFSET2+0x04, STD, 0, "2_Digital_Input_Stat", DIGI_map, 8, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET2+0x05, STD, 0, "2_Motor_Position_Info", MOTORPOS_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET2+0x06, STD, 0, "2_Current_Info", CURRENTINF_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET2+0x07, STD, 0, "2_Voltage_Info", VOLTINF_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET2+0x08, STD, 0, "2_Flux_Info", FLUXINF_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET2+0x09, STD, 0, "2_Internal_Volt", INTVOLT_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET2+0x0A, STD, 0, "2_Internal_States", NULL, 0, CAN_parser_INTST},
		{CAN_ID_OFFSET2+0x0B, STD, 0, "2_Fault_Codes", FAULTCODES_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET2+0x0C, STD, 0, "2_Torque_Timer_Info", TORQTIM_map, 3, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET2+0x0D, STD, 0, "2_Mod_Indx_FluxWeak", MODFLUX_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET2+0x0E, STD, 0, "2_Firm_Info", FIRMINF_map, 4, CAN_parser_std_Rinehart},
		{CAN_ID_OFFSET2+0x0F, STD, 0, "2_Diagnostic", NULL, 0, CAN_parser_DIAGNOSTIC},
		{CAN_ID_OFFSET2+0x22, STD, 0, "2_Parameter_Response", PARAMETER_RESPONSE_map, 0, CAN_parser_std_Rinehart},

};


/* FreeRTOS Message Queues */
static QueueHandle_t RxQueue = NULL;
static QueueHandle_t TxQueue = NULL;

/* Semaphores ----------------------------------------------------------------*/
volatile SemaphoreHandle_t CAN_Inputs_Vector_Mutex;
volatile SemaphoreHandle_t CAN_Outputs_Vector_Mutex;

/* CAN Inputs Vector ----------------------------------------------------------------*/
volatile uint32_t CAN_inputs[NUM_INPUTS];

/* Task Functions ---------------------------------------------------------------*/

/**
 * @brief	Reads incoming CAN messages from RxQueue and adds to data table
 *
 */
void CAN_Rx_Task(void * parameters)
{
	queue_msg_t Rx_msg;

	while(1)
	{
		/* get message from queue */
		xQueueReceive( RxQueue, &Rx_msg, portMAX_DELAY );

		/* filter messages */
		uint8_t done = 0;
		uint8_t i = 0;

		/* search through CAN dictionary until message is found */
		while(i < sizeof(CAN_dict)/sizeof(CAN_msg_t) && !done)
		{

			if (Rx_msg.Rx_header.StdId == CAN_dict[i].msg_ID)
			{

					CAN_dict[i].parser(Rx_msg, i);	// call message parser
					done = 1;


			}
			i++;
		}



	}
}

/**
 * @brief 	Reads outgoing CAN messages from TxQueue and sends to CAN bus
 */
void CAN_Tx_Task(void * parameters)
{
	queue_msg_t Tx_msg;
	while(1)
	{
		/* get message from queue */
		xQueueReceive( TxQueue, &Tx_msg, portMAX_DELAY );

		if (HAL_CAN_AddTxMessage(&CanHandle, &Tx_msg.Tx_header, Tx_msg.data._8, &TxMailbox) != HAL_OK)
		{
			BSP_LED_On(LED_RED);
		}
	}
}


/* Non-Task Functions ---------------------------------------------------------------*/

void CAN_Send(queue_msg_t Tx_msg)
{
	/* TODO: check that CAN message is valid */
	if (xQueueSend(TxQueue, &Tx_msg, portMAX_DELAY) != pdPASS)
	{

		BSP_LED_On(LED_RED);
	}

}

/**
 * @brief standard parser for unpacking CAN functions into CAN_inputs table (big endian to big endian)
 * @param q_msg: incoming CAN message
 * @param CAN_msg: reference message from CAN_dict w/ message metadata
 */
static void CAN_parser_std(queue_msg_t q_msg, uint8_t CAN_idx)
{
	volatile int FLAG = 0;
	/* iterate over all inputs in data field */
	for (int i = 0; i < CAN_dict[CAN_idx].num_inputs; i++)
	{
		uint32_t result = 0;
		input_map_t input = CAN_dict[CAN_idx].input_map[i];

		/* iterate over all bytes of input */
		for (int j = 0; j < input.size; j++)
		{
			result = result << 8 | (uint32_t) (q_msg.data._8[input.start_byte + j] << input.start_bit);
		}


		xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);	//get CAN inputs mutex

		/* store result in CAN_inputs table */
		CAN_inputs[input.index] = result;

		xSemaphoreGive(CAN_Inputs_Vector_Mutex);	//give CAN inputs mutex
	}
}


/**
 * @brief standard parser for unpacking Rinehart messages (and other little endian messages) into CAN_inputs table (little endian to big endian)
 * @param q_msg: incoming CAN message
 * @param CAN_msg: reference message from CAN_dict w/ message metadata
 */
static void CAN_parser_std_Rinehart(queue_msg_t q_msg, uint8_t CAN_idx) //verified good
{
	volatile int FLAG = 0;
	/* iterate over all inputs in data field */


	for (int i = 0; i < CAN_dict[CAN_idx].num_inputs; i++)
	{

		uint32_t result = 0;
		input_map_t input = CAN_dict[CAN_idx].input_map[i];
		int offset = 0;

		//Set a offset variable if the message is from Rinehart pm100 2
		if( (CAN_ID_OFFSET2 <= CAN_dict[CAN_idx].msg_ID) & (CAN_dict[CAN_idx].msg_ID <= (CAN_ID_OFFSET2 +0x0F)))
		{

			//Set the offset variable to the number of CAN inputs from one Rinehart
			offset = OFFSET_END - OFFSET_BEGIN + 1;

		}


		/* iterate over all bytes of input */
		for (int j = input.size - 1; j >=0; j--)
		{

			result = result << 8 | (uint32_t) (q_msg.data._8[input.start_byte + j] << input.start_bit);
		}

		xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);
		/* store result in CAN_inputs table */
		CAN_inputs[input.index + offset] = result;

		xSemaphoreGive(CAN_Inputs_Vector_Mutex);	//give CAN inputs mutex
	}


}


/**
 * @brief special parser for EMUS1 CAN message
 * @param q_msg: incoming CAN message
 * @param CAN_msg: reference message from CAN_dict w/ message metadata
 */
static void CAN_parser_EMUS1(queue_msg_t q_msg, uint8_t CAN_idx)
{
	xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);	//get CAN inputs mutex

	CAN_inputs[MIN_CELL_VOLTAGE] = q_msg.data._8[0];
	CAN_inputs[MAX_CELL_VOLTAGE] = q_msg.data._8[1];
	CAN_inputs[AVG_CELL_VOLTAGE] = q_msg.data._8[2];
	CAN_inputs[BATT_VOLTAGE] = (uint32_t) q_msg.data._8[5] << 24 | (uint32_t) q_msg.data._8[6] << 16 | (uint32_t) q_msg.data._8[3] << 8 | (uint32_t) q_msg.data._8[4];

	xSemaphoreGive(CAN_Inputs_Vector_Mutex);	//give CAN inputs mutex
}

/**
 * @brief special parser for Rinehart pm-100 Analog Input Voltages broadcast messages
 * @param q_msg: incoming CAN message
 * @param CAN_msg: reference message from CAN_dict w/ message metadata
 */
static void CAN_parser_ANALOGVOLT(queue_msg_t q_msg, uint8_t CAN_idx){

	int offset = 0;
	//Set a offset variable if the message is from Rinehart pm100 2
	if( (CAN_ID_OFFSET2 <= CAN_dict[CAN_idx].msg_ID) & (CAN_dict[CAN_idx].msg_ID <= (CAN_ID_OFFSET2 +0x0F))){
		//Set the offset variable to the number of CAN inputs from one Rinehart
		offset = OFFSET_END - OFFSET_BEGIN + 1;

	}

	xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);	//get CAN inputs mutex



	//store individual analog inputs in CAN_inputs array

	//seperate the inputs into the first 32 and second 32 bits in order (... 8 7 6 5 4 3 2 1 0) instead (7 6 5 4 3 2 1 0 8 ...)
	uint32_t first32bits = (uint32_t) q_msg.data._8[3] << 24 | (uint32_t) q_msg.data._8[2] << 16 | (uint32_t) q_msg.data._8[1] << 8 | (uint32_t) q_msg.data._8[0];
	uint32_t second32bits = (uint32_t) q_msg.data._8[7] << 24 | (uint32_t) q_msg.data._8[6] << 16 | (uint32_t) q_msg.data._8[5] << 8 | (uint32_t) q_msg.data._8[4];


	//do inputs 1 - 3
	for(int i = 0; i<3; i++){
		uint32_t temp = first32bits << 22;
		CAN_inputs[ANALOG_INPUT_1 + offset + i] = temp >> 22;
		first32bits = first32bits >> 10;

	}

	//do inputs 4-7
	for(int i = 0; i<3; i++){
		uint32_t temp = second32bits << 22;
		CAN_inputs[ANALOG_INPUT_4 + offset + i] = temp >> 22;
		second32bits = second32bits >> 10;

	}

	xSemaphoreGive(CAN_Inputs_Vector_Mutex);	//give CAN inputs mutex
}
/**
 * @brief special parser for Rinehart pm-100 INTERNAL STATUS broadcast messages
 * @param q_msg: incoming CAN message
 * @param CAN_msg: reference message from CAN_dict w/ message metadata
 */

static void CAN_parser_INTST(queue_msg_t q_msg, uint8_t CAN_idx){

	int offset = 0;
	//Set a offset variable if the message is from Rinehart pm100 2
	if( (CAN_ID_OFFSET2 <= CAN_dict[CAN_idx].msg_ID) & (CAN_dict[CAN_idx].msg_ID <= (CAN_ID_OFFSET2 +0x0F))){
		//Set the offset variable to the number of CAN inputs from one Rinehart
		offset = OFFSET_END - OFFSET_BEGIN + 1;

	}

	xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);	//get CAN inputs mutex


	CAN_inputs[VSM_STATE+offset] = (uint32_t) q_msg.data._8[1] << 8 | (uint32_t) q_msg.data._8[0];
	CAN_inputs[INVERTER_STATE+offset] = (uint32_t) q_msg.data._8[2];
	CAN_inputs[RELAY_STATE+offset] = (uint32_t) q_msg.data._8[3];
	CAN_inputs[INVERTER_RUN_MODE + offset] = (uint32_t) (1 & q_msg.data._8[4]);
	CAN_inputs[INVERTER_ACTIVE_DISCHARGE_STATE + offset] = (uint32_t) (0xE0 & q_msg.data._8[4]) >> 5; //TODO: ask leina if this works
	CAN_inputs[INVERTER_COMMAND_MODE + offset] = (uint32_t) q_msg.data._8[5];
	CAN_inputs[INVERTER_ENABLE_STATE + offset] = (uint32_t) (1 & q_msg.data._8[6]);
	CAN_inputs[INVERTER_ENABLE_LOCKOUT + offset] = (uint32_t) (1 & (q_msg.data._8[6]>>7));
	CAN_inputs[DIRECTION_COMMAND + offset] = (uint32_t) (1 & (q_msg.data._8[7]>>0));
	CAN_inputs[BMS_ACTIVE + offset] = (uint32_t) (1 & (q_msg.data._8[7]>>1));
	CAN_inputs[BMS_LIMITING_TORQUE + offset] = (uint32_t) (1 & (q_msg.data._8[7]>>2));

	xSemaphoreGive(CAN_Inputs_Vector_Mutex);	//give CAN inputs mutex
}


/**
 * @brief special parser for Rinehart pm-100 DIAGNOSTIC broadcast messages
 * @param q_msg: incoming CAN message
 * @param CAN_msg: reference message from CAN_dict w/ message metadata
 */
static void CAN_parser_DIAGNOSTIC(queue_msg_t q_msg, uint8_t CAN_idx){

	int offset = 0;
	//Set a offset variable if the message is from Rinehart pm100 2
	if( (CAN_ID_OFFSET2 <= CAN_dict[CAN_idx].msg_ID) & (CAN_dict[CAN_idx].msg_ID <= (CAN_ID_OFFSET2 +0x0F))){
		//Set the offset variable to the number of CAN inputs from one Rinehart
		offset = OFFSET_END - OFFSET_BEGIN + 1;

	}
	xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);	//get CAN inputs mutex

	CAN_inputs[DIAGNOSTIC_DATA_LO+offset] = (uint32_t) q_msg.data._8[0] << 24 | (uint32_t) q_msg.data._8[1] << 16 |(uint32_t) q_msg.data._8[2] << 8 | (uint32_t) q_msg.data._8[3];

	CAN_inputs[DIAGNOSTIC_DATA_LO+offset] = (uint32_t) q_msg.data._8[4] << 24 | (uint32_t) q_msg.data._8[5] << 16 |(uint32_t) q_msg.data._8[6] << 8 | (uint32_t) q_msg.data._8[7];

	xSemaphoreGive(CAN_Inputs_Vector_Mutex);	//give CAN inputs mutex
}



/**
 * @brief	Initialize CAN
 */
void CAN_Init(void)
{
	/* Configure the CAN peripheral */
	CAN_Config();

	/* Initialize Tx and Rx Queues */
	RxQueue = xQueueCreate(CAN_Rx_QUEUE_LENGTH, sizeof(queue_msg_t));
	if (RxQueue == NULL)
	{

	}

	TxQueue = xQueueCreate(CAN_Tx_QUEUE_LENGTH, sizeof(queue_msg_t));
	if (TxQueue == NULL)
	{

	}

	/* Initialize CAN Input and Output Vector Mutex's */
	CAN_Inputs_Vector_Mutex = xSemaphoreCreateMutex();
	if (CAN_Inputs_Vector_Mutex == NULL)
	{

	}

	CAN_Outputs_Vector_Mutex = xSemaphoreCreateMutex();
	if (CAN_Outputs_Vector_Mutex == NULL)
	{

	}

}


/**
  * @brief		Configures the CAN
  * @param		None
  * @retval		None
  */
static void CAN_Config(void)
{
	CAN_FilterTypeDef  sFilterConfig;

	/* 1. Configure the CAN peripheral ---------------------------------------------*/
	CanHandle.Instance = CANx;

	CanHandle.Init.TimeTriggeredMode = DISABLE;
	CanHandle.Init.AutoBusOff = DISABLE;
	CanHandle.Init.AutoWakeUp = DISABLE;
	CanHandle.Init.AutoRetransmission = ENABLE;
	CanHandle.Init.ReceiveFifoLocked= DISABLE;
	CanHandle.Init.TransmitFifoPriority = DISABLE;
	CanHandle.Init.Mode = CAN_MODE_NORMAL;
	CanHandle.Init.SyncJumpWidth = CAN_SJW_1TQ;
	CanHandle.Init.TimeSeg1 = CAN_BS1_14TQ;
	CanHandle.Init.TimeSeg2 = CAN_BS2_3TQ;
	CanHandle.Init.Prescaler = 6;

	if (HAL_CAN_Init(&CanHandle) != HAL_OK)
	{
		/* Initialization Error */

	}

	/* 2. Configure the CAN Filer ----------------------------------------------------*/
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;

	if (HAL_CAN_ConfigFilter(&CanHandle, &sFilterConfig) != HAL_OK)
	{
		/* Filter Configuration Error */

	}


	/* 3. Start the CAN peripheral ---------------------------------------------------*/
	if (HAL_CAN_Start(&CanHandle) != HAL_OK)
	{
		/* Start Error */

	}

	/* 4. Activate CAN RX notification -----------------------------------------------*/
	if (HAL_CAN_ActivateNotification(&CanHandle, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
		/* Notification Error */

	}

}


/**
  * @brief  Rx Fifo 0 message pending callback
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef		RxHeader;
	uint8_t					RxData[8];

	queue_msg_t Rx_msg;

	/* Get RX message */
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &(RxHeader), RxData) != HAL_OK)
	{

		BSP_LED_On(LED_RED);
	}

	/* Add message to RxQueue */
	Rx_msg.Rx_header = RxHeader;
	for (int i=0; i<sizeof(RxData); i++)	{
	  Rx_msg.data._8[i] = RxData[i];
	}
	if (xQueueSendFromISR(RxQueue, &Rx_msg, NULL) != pdPASS)
	{

		BSP_LED_On(LED_RED);
	}

}


/**
  * @brief CAN MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - NVIC configuration for DMA interrupt request enable
  * @param hcan: CAN handle pointer
  * @retval None
  */
void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
  GPIO_InitTypeDef   GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* CAN1 Periph clock enable */
  CANx_CLK_ENABLE();
  /* Enable GPIO clock ****************************************/
  CANx_GPIO_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* CAN1 TX GPIO pin configuration */
  GPIO_InitStruct.Pin = CANx_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Alternate =  CANx_TX_AF;

  HAL_GPIO_Init(CANx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* CAN1 RX GPIO pin configuration */
  GPIO_InitStruct.Pin = CANx_RX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Alternate =  CANx_RX_AF;

  HAL_GPIO_Init(CANx_RX_GPIO_PORT, &GPIO_InitStruct);


#if CVC_PROTOTYPE == 0

  /* CAN1 SC GPIO pin configuration */
  GPIO_InitStruct.Pin = CANx_SC_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;

  HAL_GPIO_Init(CANx_SC_GPIO_PORT, &GPIO_InitStruct);


#endif /* CVC_PROTOTYPE == 0 */


  /*##-3- Configure the NVIC (Interrupt) #################################################*/
  /* NVIC configuration for CAN1 Reception complete interrupt */
  HAL_NVIC_SetPriority(CANx_RX_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
  HAL_NVIC_EnableIRQ(CANx_RX_IRQn);
}

