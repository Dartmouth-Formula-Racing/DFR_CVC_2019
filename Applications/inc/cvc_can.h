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
	BAMO_STATUS = 0,	// 0
	MOTOR_RPM,			// 1
	MOTOR_CURRENT,		// 2
	MOTOR_TORQUE,		// 3
	MOTOR_VOLTAGE,		// 4
	MOTOR_TEMP,			// 5
	BAMO_FAULT,			// 6
	BAMO_BUS_VOLTAGE,	// 7
	BAMO_D_1_OUT_1,		// 8
	BAMO_D_1_OUT_2,		// 9
	ENG_REV_COUNT,		// 10
	ENG_RPM,			// 11
	ENG_TPS,			// 12, Throttle Position %
	ENG_MAP,			// 13, Manifold Air Pressure
	ENG_TEMP,			// 14, Engine coolant temperature
	AIR_TEMP,			// 15
	OIL_TEMP,			// 16, Engine oil temperature
	KL15_BATT_VOLTAGE,	// 17
	KL30_BATT_VOLTAGE,	// 18
	BARO,				// 19
	FLAG_OVERHEAT,		// 20
	ACTIVE_MAP,			// 21
	MIN_CELL_VOLTAGE,	// 22
	MAX_CELL_VOLTAGE,	// 23
	AVG_CELL_VOLTAGE,	// 24
	BATT_VOLTAGE,		// 25
	MIN_CELL_TEMP,		// 26
	MAX_CELL_TEMP,		// 27
	AVG_CELL_TEMP,		// 28
	BATT_CURRENT,		// 29
	BATT_CHARGE,		// 30
	BATT_SOC,			// 31

	NUM_INPUTS,			// 32, Must be last!

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
	uint32_t 		reg_ID;			// reg_ID (for bamocar messages)
	CAN_data_t		data;			// Message data
	char 			name[10];		// internal message name
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
