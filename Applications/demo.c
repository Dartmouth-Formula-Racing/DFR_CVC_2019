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
#include "cvc_serial.h"


void demoTask(void * parameters)	{

	/* Initialize board LED */
	BSP_LED_Init(LED_BLUE);

	for( ;; )
	{
		/* Place this task in the blocked state until it is time to run again. */
		vTaskDelay((TickType_t) 1000/portTICK_PERIOD_MS);

		BSP_LED_Toggle(LED_BLUE);
		//uint8_t demoMessage[] = "\n\r toggled \n\r";
		//uint8_t ubSizeToSend = sizeof(demoMessage);

		UserButton_Callback();
	}
}
