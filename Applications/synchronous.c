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
	vTaskDelay((TickType_t) 10/portTICK_PERIOD_MS);


/*
	SPI_outputs_vector.safety = 1;
	SPI_outputs_vector.ready_to_drive = 0;
	SPI_outputs_vector.rfg = 0;
	SPI_outputs_vector.ignition_kill = 1;
	SPI_outputs_vector.downshift_solenoid = 0;
	SPI_outputs_vector.upshift_solenoid = 0;

	SPI_Routine();

*/

}


/**
  * @brief	Slow synchronous task (20 Hz)
  *
  */
void _50_ms_Task(void * parameters)
{
	vTaskDelay((TickType_t) 50/portTICK_PERIOD_MS);


}
