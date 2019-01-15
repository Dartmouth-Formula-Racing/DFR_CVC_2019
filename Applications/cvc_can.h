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

/* Defines ------------------------------------------------------------------------*/
/* Definition for CANx clock resources */
#define CANx						CAN1
#define CANx_CLK_ENABLE() 			__HAL_RCC_CAN1_CLK_ENABLE()
#define CANx_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOB_CLK_ENABLE()

#define CANx_FORCE_RESET()			__HAL_RCC_CAN1_FORCE_RESET()
#define CANx_RELEASE_RESET()		__HAL_RCC_CAN1_RELEASE_RESET()

/* Definition for CANx Pins */
#define CANx_TX_PIN					GPIO_PIN_9
#define CANx_TX_GPIO_PORT			GPIOB
#define CANx_TX_AF					GPIO_AF9_CAN1

#define CANx_RX_PIN					GPIO_PIN_8
#define CANx_RX_GPIO_PORT			GPIOB
#define CANx_RX_AF					GPIO_AF9_CAN1

/* Definition for CAN's NVIC */
#define CANx_RX_IRQn				CAN1_RX0_IRQn
#define CANx_RX_IRQHandler			CAN1_RX0_IRQHandler

/* Definitions for Queues */
#define CAN_Rx_QUEUE_LENGTH			100U
#define CAN_Tx_QUEUE_LENGTH			100U

/* Definitions for CAN identifiers */
#define STD							CAN_ID_STD
#define EXT							CAN_ID_EXT

/* Type Definitions ------------------------------------------------------------------------*/
typedef struct bamocar_data_16_s
{
	uint8_t		reg_ID;
	uint16_t	data;
} bamocar_data_16_t;

typedef union CAN_data_u
{
	uint64_t	_64;
	uint32_t	_32[2];
	uint16_t	_16[4];
	uint8_t		_8[8];
	float		_float[2];
	double		_double;

	bamocar_data_16_t	bamocar_data_16;

} CAN_data_t;


typedef struct CAN_msg_s
{
	uint32_t	msg_ID;		// Message ID
	uint32_t	msg_type;	// STD or EXT
	uint32_t 	reg_ID;		// reg_ID (for bamocar messages)
	CAN_data_t	data;		// Message data
	char 		name[10];	// internal message name

} CAN_msg_t;

/* Function Prototypes ------------------------------------------------------------------------*/
void CAN_Init(void)	;
HAL_StatusTypeDef CAN_Polling(void);
void Error_Handler(void);
void CAN_Demo_Task(void * parameters);
void CAN_Rx_Task(void * parameters);
void CAN_Tx_Task(void * parameters);

/* Variables to Export ------------------------------------------------------------------------*/
typedef enum input_index_e
{
	BAMO_STATUS = 0,
	MOTOR_RPM,
	MOTOR_CURRENT,
	MOTOR_TORQUE,
	MOTOR_VOLTAGE,
	MOTOR_TEMP,
	BAMO_FAULT,
	BAMO_BUS_VOLTAGE,
	BAMO_D_1_OUT_1,
	BAMO_D_1_OUT_2,
	ENG_REV_COUNT,
	ENG_RPM,
	ENG_TPS,			// Throttle Position %
	ENG_MAP,			// Manifold Air Pressure
	ENG_TEMP,
	AIR_TEMP,
	OIL_TEMP,
	KL15_BATT_VOLTAGE,
	KL30_BATT_VOLTAGE,
	BARO,
	FLAG_OVERHEAT,
	ACTIVE_MAP,
	MIN_CELL_VOLTAGE,
	MAX_CELL_VOLTAGE,
	AVG_CELL_VOLTAGE,
	TOTAL_VOLTAGE,
	MIN_CELL_TEMP,
	MAX_CELL_TEMP,
	AVG_CELL_TEMP,
	BATT_CURRENT,
	BATT_CHARGE,
	BATT_SOC,

	NUM_INPUTS,			// Must be last!

} input_index_t;

uint32_t CAN_inputs[NUM_INPUTS];


#endif /* CVC_CAN_H_ */
