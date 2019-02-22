/*
 * demo.c
 *
 *  Created on: Dec 1, 2018
 *      Author: f002bc7
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx_nucleo_144.h"
#include "FreeRTOS.h"
#include "task.h"

void demoTask(void * parameters)	{

	/* Initialize board LED */
	BSP_LED_Init(LED_BLUE);

	for( ;; )
	{
		/* Place this task in the blocked state until it is time to run again. */
		vTaskDelay((TickType_t) 1000/portTICK_PERIOD_MS);

		BSP_LED_Toggle(LED_BLUE);
	}
}

/**
  * @brief	Turns on/off the dedicated LED
  * @param	LedStatus: LED number from 1 to 3
  * @retval	None
  */
void LED_Display(uint8_t LedStatus)
{
	BSP_LED_Off(LED1);
	BSP_LED_Off(LED2);
	BSP_LED_Off(LED3);

	switch(LedStatus)
	{
		case (1):
			BSP_LED_On(LED1);
			break;
		case (2):
			BSP_LED_On(LED2);
			break;
		case (3):
			BSP_LED_On(LED3);
			break;
		default:
			break;
	}
}
