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
#include "cvc_state_machine.h"



//#define PWM_PIN						GPIO_PIN_6
//#define PWM_PORT					GPIOB
//#define PWM_AF						GPIO_MODE_AF_PP
//#define PWM_SPEED					GPIO_SPEED_FREQ_LOW

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor TIMx instance used and associated
   resources */
/* Definition for TIMx clock resources */
#define TIMx                           TIM4

#define TIMx_CLK_ENABLE()              __HAL_RCC_TIM4_CLK_ENABLE()

/* Definition for TIMx Channel Pins */
#define TIMx_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define TIMx_GPIO_PORT		           GPIOB
#define TIMx_GPIO_PIN                  GPIO_PIN_6
#define TIMx_GPIO_AF		           GPIO_AF2_TIM4

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* Private typedef -----------------------------------------------------------*/
#define	PERIOD_VALUE       	(uint32_t) (8000)  /* Period Value  */
#define MIN_PULSE_VALUE		(uint32_t) (PERIOD_VALUE*.025 + 400)
#define MID_PULSE_VALUE		(uint32_t) (PERIOD_VALUE*.5)
#define MAX_PULSE_VALUE    	(uint32_t) (PERIOD_VALUE*.125 - 200)  /* Period Value  */


/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle;

void pwm_init(void);
void pwm_timer_init(void);
void set_duty(uint32_t duty_input);

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *);


#endif /* INC_CVC_PWM_H_ */
