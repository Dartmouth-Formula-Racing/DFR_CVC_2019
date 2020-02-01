/*
 * synchronous.c
 *
 *  Created on: Dec 12, 2018
 *      Author: f002ccx
 */

/* Includes ------------------------------------------------------------------*/
#include "synchronous.h"
#include "pm100.h"

/**
  * @brief Fast synchronous task (100 Hz)
  */
void _10_ms_Task(void * parameters)
{
	TickType_t LastWakeTime = xTaskGetTickCount();

	while(1)
	{
		vTaskDelayUntil(&LastWakeTime, (TickType_t) 10/portTICK_PERIOD_MS);



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

		log_data();
		parameter_read_command_1(143);

	}

}
