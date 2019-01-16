/*
 * synchronous.c
 *
 *  Created on: Dec 12, 2018
 *      Author: f002ccx
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx_nucleo_144.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cvc_spi.h"

/* Volatile variables --------------------------------------------------------*/


/**
  * @brief Fast synchronous task (100 Hz)
  */
void _10_ms_Task(void * parameters)
{
	while(1)
	{
		vTaskDelay((TickType_t) 10/portTICK_PERIOD_MS);


		initiate_SPI_transmission();
	}
}


/**
  * @brief	Slow synchronous task (20 Hz)
  *
  */
void _50_ms_Task(void * parameters)
{
	vTaskDelay((TickType_t) 50/portTICK_PERIOD_MS);


}
