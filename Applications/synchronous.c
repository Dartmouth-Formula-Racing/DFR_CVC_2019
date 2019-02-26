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
	while(1)
	{
		vTaskDelay((TickType_t) 10/portTICK_PERIOD_MS);

		TickType_t start = xTaskGetTickCount();
//		state_machine();
//
//		if (cvc_state == DRIVE)
//		{
//			torque_command();
//			safety_monitor();
//		}

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
void _50_ms_Task(void * parameters)
{

	while(1)
	{
		vTaskDelay((TickType_t) 50/portTICK_PERIOD_MS);

#if LOGGING_TEST

		log_data();

#else
		/* Only log in DRIVE state */
		if (cvc_state == DRIVE)
		{
			log_data();
		}
#endif	/* LOGGING_TEST */

	}

}
