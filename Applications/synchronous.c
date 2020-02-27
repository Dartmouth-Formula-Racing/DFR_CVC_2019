/*
 * synchronous.c
 *
 *  Created on: Dec 12, 2018
 *      Author: f002ccx
 */

/* Includes ------------------------------------------------------------------*/
#include "synchronous.h"
#include "cvc_can.h"

/**
  * @brief Fast synchronous task (100 Hz)
  */
void _10_ms_Task(void * parameters)
{
	TickType_t LastWakeTime = xTaskGetTickCount();

	while(1)
	{
		vTaskDelayUntil(&LastWakeTime, (TickType_t) 10/portTICK_PERIOD_MS);


		TickType_t start = xTaskGetTickCount();
		if(HAL_GPIO_ReadPin(B1_GPIO_PORT, B1_PIN)){
			pm100_relay_command_1(1);
		}
		else{
			pm100_relay_command_1(0);
		}
		//torque_command();


		TickType_t end = xTaskGetTickCount();

		if(end > start) {
			int good = 1;
		}


	}
}


/**
  * @brief	Slow synchronous task (20 Hz)
  *
  */
void _20_ms_Task(void * parameters)
{

	TickType_t LastWakeTime = xTaskGetTickCount();

	while(1)
	{
		vTaskDelayUntil(&LastWakeTime, (TickType_t) 20/portTICK_PERIOD_MS);

	}

}
