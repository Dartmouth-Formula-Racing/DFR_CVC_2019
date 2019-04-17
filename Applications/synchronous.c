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
		vTaskDelayUntil(&LastWakeTime, (TickType_t) 10/portTICK_PERIOD_MS);

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
		vTaskDelayUntil(&LastWakeTime, (TickType_t) 20/portTICK_PERIOD_MS);

		if (cvc_state > VOLTAGE_CHECK)
		{
			log_data();
		}
	}

}


void _50_ms_Task(void * parameters)
{
	TickType_t LastWakeTime = xTaskGetTickCount();
	int pwm = MIN_PULSE_VALUE;
	int inc = 1;

		while(1)
		{
			vTaskDelay((TickType_t) 5/portTICK_PERIOD_MS);

			// set PWM duty cycle
			__HAL_TIM_SetCompare(&TimHandle, TIM_CHANNEL_1, pwm);

			if (pwm == MAX_PULSE_VALUE && inc == 1)
			{
				inc = 0;
//				vTaskDelay((TickType_t) 100/portTICK_PERIOD_MS);
			}
			else if (pwm == MIN_PULSE_VALUE && inc == 0)
			{
				inc = 1;
//				vTaskDelay((TickType_t) 100/portTICK_PERIOD_MS);

			}

			if (inc && pwm < MAX_PULSE_VALUE)
			{
				pwm++;
			}
			else if (!inc && pwm > MIN_PULSE_VALUE)
			{
				pwm--;
			}

		}
}
