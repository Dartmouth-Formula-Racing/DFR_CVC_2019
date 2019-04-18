/*
 * cvc_pwm.c
 *
 *  Created on: Apr 4, 2019
 *      Author: f002ccx
 */


#include "cvc_pwm.h"

/* Private typedef -----------------------------------------------------------*/
#define  PULSE1_VALUE       (uint32_t)(PERIOD_VALUE/2)        /* Capture Compare 1 Value  */
#define PWM_COUNTER_FREQ	(uint32_t)(400000)			/* Counter frequency for output frequency = 50 Hz */


/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sConfig;


/* Counter Prescaler value */
uint32_t uhPrescalerValue = 0;


void pwm_init(void)
{
	HAL_TIM_PWM_MspInit(&TimHandle);

	pwm_timer_init();

}


void pwm_timer_init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();

//	uint32_t sclk = HAL_RCC_GetSysClockFreq();

	/* Compute the prescaler value to have TIM2 counter clock equal to 200000 Hz */
    uhPrescalerValue = (uint32_t)((SystemCoreClock/2) / PWM_COUNTER_FREQ) - 1;


    /*##-1- Configure the TIM peripheral #######################################*/
     /* -----------------------------------------------------------------------
     TIM4 Configuration: generate 4 PWM signals with 4 different duty cycles.

       In this example TIM4 input clock (TIM4CLK) is set to APB1 clock x 2,
       since APB1 prescaler is equal to 4.
         TIM4CLK = APB1CLK*2
         APB1CLK = HCLK/4
         => TIM4CLK = HCLK/2 = SystemCoreClock/2

       To get TIM4 counter clock at 21.6 MHz, the prescaler is computed as follows:
          Prescaler = (TIM4CLK / TIM4 counter clock) - 1
          Prescaler = ((SystemCoreClock/2) /21.6 MHz) - 1

       To get TIM4 output clock at 50 Hz, the period (ARR)) is computed as follows:
          ARR = (TIM4 counter clock / TIM4 output clock) - 1
              = 431999

       Note:
        SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f7xx.c file.
        Each time the core clock (HCLK) changes, user had to update SystemCoreClock
        variable value. Otherwise, any configuration based on this variable will be incorrect.
        This variable is updated in three ways:
         1) by calling CMSIS function SystemCoreClockUpdate()
         2) by calling HAL API function HAL_RCC_GetSysClockFreq()
         3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
     ----------------------------------------------------------------------- */

     /* Initialize TIMx peripheral as follows:
          + Prescaler = ((SystemCoreClock/2) / 21600000) - 1
          + Period = (432000 - 1)
          + ClockDivision = 0
          + Counter direction = Up
     */

    TimHandle.Instance = TIMx;

	TimHandle.Init.Prescaler         = uhPrescalerValue;
	TimHandle.Init.Period            = PERIOD_VALUE;
	TimHandle.Init.ClockDivision     = 0;
	TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	TimHandle.Init.RepetitionCounter = 0;
	TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;


	if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
	{
	    init_fault_handler();
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
	sConfig.Pulse = PERIOD_VALUE;
	if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIMx_CHANNEL) != HAL_OK)
	{
		/* Configuration Error */
		init_fault_handler();
	}

	/* Start channel 1 */
	if (HAL_TIM_PWM_Start(&TimHandle, TIMx_CHANNEL) != HAL_OK)
	{
		/* PWM Generation Error */
		init_fault_handler();
	}


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

  GPIO_InitStruct.Alternate = TIMx_GPIO_AF;
  GPIO_InitStruct.Pin = TIMx_GPIO_PIN;
  HAL_GPIO_Init(TIMx_GPIO_PORT, &GPIO_InitStruct);

}
