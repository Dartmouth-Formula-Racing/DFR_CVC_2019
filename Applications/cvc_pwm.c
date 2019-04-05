/*
 * cvc_pwm.c
 *
 *  Created on: Apr 4, 2019
 *      Author: f002ccx
 */


#include "cvc_pwm.h"


void pwm_timer_init(void)
{

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 19;
	htim2.Init.Period = 1000;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

	if (HAL_TIM_BASE_INIT(&htim2) != HAL_OK)
	{

	}


}


void set_duty(uint32_t duty_input)
{

}
