/*
 * cvc_pwm.c
 *
 *  Created on: Apr 4, 2019
 *      Author: f002ccx
 */


#include "cvc_pwm.h"

/* Private typedef -----------------------------------------------------------*/
#define  PERIOD_VALUE       (uint32_t)(900 - 1)  /* Period Value  */
#define  PULSE1_VALUE       (uint32_t)(PERIOD_VALUE/2)        /* Capture Compare 1 Value  */

/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sConfig;


/* Counter Prescaler value */
uint32_t uhPrescalerValue = 0;


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

	/* Compute the prescaler value to have TIM2 counter clock equal to 21600000 Hz */
    uhPrescalerValue = (uint32_t)((SystemCoreClock/2) / 21600000) - 1;

    TimHandle.Instance = TIMx;

	TimHandle.Init.Prescaler         = uhPrescalerValue;
	TimHandle.Init.Period            = PERIOD_VALUE;
	TimHandle.Init.ClockDivision     = 0;
	TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	TimHandle.Init.RepetitionCounter = 0;
	TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;


	if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
	{

	}


	/*##-2- Configure the PWM channels #########################################*/
	/* Common configuration for all channels */
	sConfig.OCMode       = TIM_OCMODE_PWM1;
	sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
	sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
	sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
	sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;

	 /* Set the pulse value for channel 1 */
	  sConfig.Pulse = PULSE1_VALUE;
	  if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK)
	  {
	    /* Configuration Error */
	    Error_Handler();
	  }


	  HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *TimHandle);


}


void set_duty(uint32_t duty_input)
{

}




/* MSP functions ------------------------------------------------------------- */
/**
  * @brief TIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef   GPIO_InitStruct;
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  TIMx_CLK_ENABLE();

  /* Enable all GPIO Channels Clock requested */
  TIMx_CHANNEL_GPIO_PORT();

  /* Configure PB.06  (TIM4_Channel1), PB.07  (TIM4_Channel2), PB.08  (TIM4_Channel3),
     PB.09  (TIM4_Channel4) in output, push-pull, alternate function mode
  */
  /* Common configuration for all channels */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  GPIO_InitStruct.Alternate = TIMx_GPIO_AF_CHANNEL1;
  GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL1;
  HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL1, &GPIO_InitStruct);

  GPIO_InitStruct.Alternate = TIMx_GPIO_AF_CHANNEL2;
  GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL2;
  HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL2, &GPIO_InitStruct);

  GPIO_InitStruct.Alternate = TIMx_GPIO_AF_CHANNEL3;
  GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL3;
  HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL3, &GPIO_InitStruct);

  GPIO_InitStruct.Alternate = TIMx_GPIO_AF_CHANNEL4;
  GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL4;
  HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL4, &GPIO_InitStruct);
}
