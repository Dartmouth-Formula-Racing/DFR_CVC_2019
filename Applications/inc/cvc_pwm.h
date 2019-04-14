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
//#define CANx						CAN1
//#define CANx_CLK_ENABLE() 			__HAL_RCC_CAN1_CLK_ENABLE()

#define PWM_PIN						GPIO_PIN_6
#define PWM_PORT					GPIOB
#define PWM_AF						GPIO_MODE_AF_PP
#define PWM_SPEED					GPIO_SPEED_FREQ_LOW


TIM_HandleTypeDef htim4;

void pwm_init(void);
void pwm_timer_init(void);
void set_duty(uint32_t duty_input);


void HAL_MspInit(void);
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base);


#endif /* INC_CVC_PWM_H_ */
