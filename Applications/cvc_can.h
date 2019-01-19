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
typedef enum input_index_e
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
	ENG_TEMP,			// 14
	AIR_TEMP,			// 15
	OIL_TEMP,			// 16
	KL15_BATT_VOLTAGE,	// 17
	KL30_BATT_VOLTAGE,	// 18
	BARO,				// 19
	FLAG_OVERHEAT,		// 20
	ACTIVE_MAP,			// 21
	MIN_CELL_VOLTAGE,	// 22
	MAX_CELL_VOLTAGE,	// 23
	AVG_CELL_VOLTAGE,	// 24
	TOTAL_VOLTAGE,		// 25
	MIN_CELL_TEMP,		// 26
	MAX_CELL_TEMP,		// 27
	AVG_CELL_TEMP,		// 28
	BATT_CURRENT,		// 29
	BATT_CHARGE,		// 30
	BATT_SOC,			// 31

	NUM_INPUTS,			// 32, Must be last!

} input_index_t;


/* Variables to Export ------------------------------------------------------------------------*/

uint32_t CAN_inputs[NUM_INPUTS];

/* Function Prototypes ------------------------------------------------------------------------*/
void CAN_Init(void)	;
HAL_StatusTypeDef CAN_Polling(void);
void Error_Handler(void);
void CAN_Demo_Task(void * parameters);
void CAN_Rx_Task(void * parameters);
void CAN_Tx_Task(void * parameters);



#endif /* CVC_CAN_H_ */
