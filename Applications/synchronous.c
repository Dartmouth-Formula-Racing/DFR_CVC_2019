/*
 * synchronous.c
 *
 *  Created on: Dec 12, 2018
 *      Author: f002ccx
 */

/* Includes ------------------------------------------------------------------*/
#include "synchronous.h"

/**
  * @brief Fast synchronous task (100 Hz)
  */
void _10_ms_Task(void * parameters)
{
	TickType_t LastWakeTime = xTaskGetTickCount();

	while(1)
	{
		vTaskDelay((TickType_t) 10/portTICK_PERIOD_MS);

#if LOGGING_TEST == 0
		TickType_t start = xTaskGetTickCount();
		state_machine();

		if (cvc_state == DRIVE)
		{
			torque_command();
			safety_monitor();
		}

		TickType_t end = xTaskGetTickCount();

		if(end > start) {
			int good = 1;
		}
#endif

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
		vTaskDelay((TickType_t) 20/portTICK_PERIOD_MS);

		if (cvc_state > VOLTAGE_CHECK)
		{
			log_data();
		}
	}

}
