/*
 * cvc_can.h
 *
 *  Created on: Nov 30, 2018
 *      Author: f002ccx
 */

#ifndef CVC_CAN_H_
#define CVC_CAN_H_

/* Includes ------------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_can.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "cvc_config.h"

/* Defines ------------------------------------------------------------------------*/
/* Definition for CANx clock resources */
#define CANx						CAN1
#define CANx_CLK_ENABLE() 			__HAL_RCC_CAN1_CLK_ENABLE()

#define CANx_FORCE_RESET()			__HAL_RCC_CAN1_FORCE_RESET()
#define CANx_RELEASE_RESET()		__HAL_RCC_CAN1_RELEASE_RESET()

/* Definition for CANx Pins */
#if CVC_PROTOTYPE == 1

#define CANx_TX_PIN					GPIO_PIN_9
#define CANx_TX_GPIO_PORT			GPIOB
#define CANx_TX_AF					GPIO_AF9_CAN1

#define CANx_RX_PIN					GPIO_PIN_8
#define CANx_RX_GPIO_PORT			GPIOB
#define CANx_RX_AF					GPIO_AF9_CAN1

#define CANx_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()

#define B1_PIN						GPIO_PIN_13
#define B1_GPIO_PORT				GPIOC
#else

#define CANx_TX_PIN					GPIO_PIN_1
#define CANx_TX_GPIO_PORT			GPIOD
#define CANx_TX_AF					GPIO_AF9_CAN1

#define CANx_RX_PIN					GPIO_PIN_0
#define CANx_RX_GPIO_PORT			GPIOD
#define CANx_RX_AF					GPIO_AF9_CAN1

#define CANx_SC_PIN					GPIO_PIN_0
#define CANx_SC_GPIO_PORT			GPIOE

#define CANx_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOD_CLK_ENABLE();\
									__HAL_RCC_GPIOE_CLK_ENABLE()


#endif /* CVC_PROTOTYPE ==1 */

/* Definition for CAN's NVIC */
#define CANx_RX_IRQn				CAN1_RX0_IRQn
#define CANx_RX_IRQHandler			CAN1_RX0_IRQHandler

/* Definitions for Queues */
#define CAN_Rx_QUEUE_LENGTH			100U
#define CAN_Tx_QUEUE_LENGTH			100U

/* Definitions for CAN identifiers */
#define STD							CAN_ID_STD
#define EXT							CAN_ID_EXT
#define RTR							CAN_RTR_DATA


/* Type Definitions ------------------------------------------------------------------------*/

/* enum for inputs vector index */

typedef enum CAN_input_e
{

	MIN_CELL_VOLTAGE = 0,	// 0
	MAX_CELL_VOLTAGE,	// 1
	AVG_CELL_VOLTAGE,	// 2
	BATT_VOLTAGE,		// 3
	MIN_CELL_TEMP,		// 4
	MAX_CELL_TEMP,		// 5
	AVG_CELL_TEMP,		// 6
	BATT_CURRENT,		// 7
	BATT_CHARGE,		// 8
	BATT_SOC,

	//individual cell temps read and sent by monitor with first number corresponding to individual cell, and second corresponding to transmitter
	CELL_TEMP_1_1,
	CELL_TEMP_2_1,
	CELL_TEMP_3_1,
	CELL_TEMP_4_1,
	CELL_TEMP_5_1,
	CELL_TEMP_6_1,

	CELL_TEMP_1_2,
	CELL_TEMP_2_2,
	CELL_TEMP_3_2,
	CELL_TEMP_4_2,
	CELL_TEMP_5_2,
	CELL_TEMP_6_2,

	CELL_TEMP_1_3,
	CELL_TEMP_2_3,
	CELL_TEMP_3_3,
	CELL_TEMP_4_3,
	CELL_TEMP_5_3,
	CELL_TEMP_6_3,

	CELL_TEMP_1_4,
	CELL_TEMP_2_4,
	CELL_TEMP_3_4,
	CELL_TEMP_4_4,
	CELL_TEMP_5_4,
	CELL_TEMP_6_4,

	CELL_TEMP_1_5,
	CELL_TEMP_2_5,
	CELL_TEMP_3_5,
	CELL_TEMP_4_5,
	CELL_TEMP_5_5,
	CELL_TEMP_6_5,

	CELL_TEMP_1_6,
	CELL_TEMP_2_6,
	CELL_TEMP_3_6,
	CELL_TEMP_4_6,
	CELL_TEMP_5_6,
	CELL_TEMP_6_6,


	//First Drive inverter, pm-100, rinehart !!PRESERVE ORDER!! if changing any of this, remember to change parsers
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
	PARAMETER_RESPONSE_DATA_2,

	//Vehicle Dynamics Module
	GPS_LATITUDE,
	GPS_LONGITUDE,
	GPS_SPEED,
	GPS_ALTITUDE,
	GPS_TRUE_COURSE,
	GPS_SATELLITES_IN_USE,
	GPS_VALID_0,
	GPS_VALID_1,
	UTC_DATE_YEAR,
	UTC_DATE_MONTH,
	UTC_DATE_DAY,
	UTC_TIME_HOURS,
	UTC_TIME_MINUTES,
	UTC_TIME_SECONDS,
	X_AXIS_ACCELERATION,
	Y_AXIS_ACCELERATION,
	Z_AXIS_ACCELERATION,
	X_AXIS_YAW_RATE,
	Y_AXIS_YAW_RATE,
	Z_AXIS_YAW_RATE,

	//Dash inputs
	DASH_DRIVE_BUTTON,
	DASH_NEUTRAL_BUTTON,
	DASH_REVERSE_BUTTON,
	DASH_TOGGLE_1,
	DASH_TOGGLE_2,
	DASH_ALIVE,



	NUM_INPUTS,			// Must be last!

} CAN_input_t;

/* CAN data union */
typedef union CAN_data_u
{
	uint64_t	_64;
	uint32_t	_32[2];
	uint16_t	_16[4];
	uint8_t		_8[8];
	float		_float[2];
	double		_double;

} CAN_data_t;

/* Struct to hold CAN input map (follows big endian) */
typedef struct input_map_s
{
	CAN_input_t 	index;			/* index in inputs array */
	uint8_t 		start_byte;		/* input start byte (MSB) in CAN data field */
	uint8_t 		start_bit;		/* input start bit */
	uint8_t 		size;			/* input size in bytes */
} input_map_t;

/* Struct to hold messages used in CAN message queues */
typedef struct queue_msg_s
{
	union
	{
		CAN_TxHeaderTypeDef	Tx_header;
		CAN_RxHeaderTypeDef	Rx_header;
	};
	CAN_data_t data;
} queue_msg_t;

/* Prototype that all CAN parser functions must use */
typedef void (*CAN_parser_t)(queue_msg_t q_msg, uint8_t CAN_idx);

/* CAN message type  */
typedef struct CAN_msg_s
{
	uint32_t		msg_ID;			// Message ID
	uint32_t		msg_type;		// STD or EXT
	CAN_data_t		data;			// Message data
	char 			name[64];		// internal message name
	input_map_t *   input_map;		// input_map for data
	uint8_t			num_inputs;		// number of inputs
	CAN_parser_t	parser;			// parser function

} CAN_msg_t;


/* Variables to Export ------------------------------------------------------------------------*/

extern volatile uint32_t CAN_inputs[NUM_INPUTS];
extern volatile SemaphoreHandle_t CAN_Inputs_Vector_Mutex;
extern volatile SemaphoreHandle_t CAN_Outputs_Vector_Mutex;

/* Function Prototypes ------------------------------------------------------------------------*/
void CAN_Init(void)	;
HAL_StatusTypeDef CAN_Polling(void);
//void Error_Handler(void);
void CAN_Rx_Task(void * parameters);
void CAN_Tx_Task(void * parameters);
void CAN_Send(queue_msg_t Tx_msg);


#endif /* CVC_CAN_H_ */
