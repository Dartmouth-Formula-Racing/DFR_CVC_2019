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

/* Definition for CANx clock resources */
#define CANx						CAN1
#define CANx_CLK_ENABLE() 			__HAL_RCC_CAN1_CLK_ENABLE()
#define CANx_GPIO_CLK_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()

#define CANx_FORCE_RESET()			__HAL_RCC_CAN1_FORCE_RESET()
#define CANx_RELEASE_RESET()		__HAL_RCC_CAN1_RELEASE_RESET()

/* Definition for CANx Pins */




#endif /* CVC_CAN_H_ */
