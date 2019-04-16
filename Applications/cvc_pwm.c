/*
 * cvc_pwm.c
 *
 *  Created on: Apr 4, 2019
 *      Author: f002ccx
 */


#include "cvc_pwm.h"

void pwm_init(void)
{
	pwm_timer_init();

	HAL_TIM_Base_Start(&htim4);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
}


void pwm_timer_init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();

	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef sConfigOC;

	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 15;
	htim4.Init.Period = 3096;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;

	if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
	{

	}

	//sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_ETRMODE1;
	if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig)!= HAL_OK)
	{

	}

	if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
	{

	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
	{

	}

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 1953;
	sConfigOC.OCPolarity =  TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{

	}

	HAL_TIM_MspPostInit(&htim4);
}


void set_duty(uint32_t duty_input)
{

}




/* MSP functions ------------------------------------------------------------- */

//void HAL_MspInit(void)
//{
//  /* USER CODE BEGIN MspInit 0 */
//
//  /* USER CODE END MspInit 0 */
//
//  __HAL_RCC_AFIO_CLK_ENABLE();
//
//  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
//
//  /* System interrupt init*/
//  /* MemoryManagement_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
//  /* BusFault_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
//  /* UsageFault_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
//  /* SVCall_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
//  /* DebugMonitor_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
//  /* PendSV_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
//  /* SysTick_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
//
//    /**DISABLE: JTAG-DP Disabled and SW-DP Disabled
//    */
//  __HAL_AFIO_REMAP_SWJ_DISABLE();
//
//  /* USER CODE BEGIN MspInit 1 */
//
//  /* USER CODE END MspInit 1 */
//}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspInit 0 */

  /* USER CODE END TIM4_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM4_CLK_ENABLE();
  /* USER CODE BEGIN TIM4_MspInit 1 */

  /* USER CODE END TIM4_MspInit 1 */
  }

}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspPostInit 0 */

  /* USER CODE END TIM4_MspPostInit 0 */

    /**TIM4 GPIO Configuration
    PB6     ------> TIM4_CH1
    */
    GPIO_InitStruct.Pin = PWM_PIN;
    GPIO_InitStruct.Mode = PWM_AF;
    GPIO_InitStruct.Speed = PWM_SPEED;
    HAL_GPIO_Init(PWM_PORT, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM4_MspPostInit 1 */

  /* USER CODE END TIM4_MspPostInit 1 */
  }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM4_MspDeInit 0 */

  /* USER CODE END TIM4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM4_CLK_DISABLE();
  }
  /* USER CODE BEGIN TIM4_MspDeInit 1 */

  /* USER CODE END TIM4_MspDeInit 1 */

}


