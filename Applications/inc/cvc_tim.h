/*
 * cvc_tim.h
 *
 *  Created on: Dec 6, 2018
 *      Author: f002ccx
 */

#ifndef INC_CVC_TIM_H_
#define INC_CVC_TIM_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx.h"
#include "stm32f7xx_nucleo_144.h"

#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_pwr.h"
#include "stm32f7xx_ll_system.h"
#include "stm32f7xx_ll_utils.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_exti.h"
#include "stm32f7xx_ll_tim.h"
#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/






/**
  * @brief  TIM1_ARR register maximum value
  */
#define TIM1_ARR_MAX (uint32_t)0xFFFF

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

void Configure_TIMInputCapture(void);
void Configure_TIMPWMOutput(void);
void Configure_Frequency(uint32_t Frequency);

/* User push-button interrupt processing */
void UserButton_Callback(void);

/* TIM1 interrupt processing */
void TimerCaptureCompare_Callback(void);

#endif /* INC_CVC_TIM_H_ */
