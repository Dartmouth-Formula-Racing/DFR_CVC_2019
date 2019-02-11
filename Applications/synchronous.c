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
extern volatile SPI_outputs_vector_t SPI_outputs_vector;

/**
  * @brief Fast synchronous task (100 Hz)
  */
void _10_ms_Task(void * parameters)
{
	while(1)
	{
		vTaskDelay((TickType_t) 10/portTICK_PERIOD_MS);

		SPI_outputs_vector.safety = SPI_inputs_vector.ICE_enable;
		SPI_outputs_vector.ready_to_drive = SPI_inputs_vector.Motor_enable;
		SPI_outputs_vector.rfg = SPI_inputs_vector.Ready_to_drive;
		SPI_outputs_vector.ignition_kill = SPI_inputs_vector.Dash_BRB_press;
		SPI_outputs_vector.downshift_solenoid = SPI_inputs_vector.IMD_safety_circuit_fault;
		SPI_outputs_vector.upshift_solenoid = SPI_inputs_vector.BMS_safety_circuit_fault;

//		initiate_SPI_transmission();
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
	}

}
