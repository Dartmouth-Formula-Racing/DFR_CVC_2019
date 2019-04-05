/*
 * cvc_pwm.h
 *
 *  Created on: Apr 4, 2019
 *      Author: f002ccx
 */

#ifndef INC_CVC_PWM_H_
#define INC_CVC_PWM_H_

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_tim.h"


/* Definition for CANx clock resources */
#define CANx						CAN1
#define CANx_CLK_ENABLE() 			__HAL_RCC_CAN1_CLK_ENABLE()

#define CANx_TX_PIN					GPIO_PIN_1
#define CANx_TX_GPIO_PORT			GPIOD
#define CANx_TX_AF					GPIO_AF9_CAN1

TIM_HandleTypeDef htim2;


void pwm_timer_init(void);
void set_duty(uint32_t duty_input);



#endif /* INC_CVC_PWM_H_ */
