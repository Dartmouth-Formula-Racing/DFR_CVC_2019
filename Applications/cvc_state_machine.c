/*
 * cvc_state_machine.c
 *
 *  Created on: Feb 12, 2019
 *      Author: f002bc7
 */

#include "cvc_state_machine.h"
#include "cvc_spi.h"
#include "cvc_can.h"

volatile cvc_state_t cvc_state = BAMO_INIT;


void state_machine()
{
	switch(cvc_state)
	{
	case BAMO_INIT:

		/* set SPI outputs */
		xSemaphoreTake(SPI_Outputs_Vector_Mutex, portMAX_DELAY);

		SPI_outputs_vector.safety = 0;
		SPI_outputs_vector.ready_to_drive = 0;
		SPI_outputs_vector.rfg = 0;

		xSemaphoreGive(SPI_Outputs_Vector_Mutex);

		/* send Bamocar reset message */
		bamo_var1_reset();

		/* send Bamocar init messages */
		if (bamo_init() == 1)
		{
			cvc_state = PRECHARGE;
		}

		break;

	case PRECHARGE:

		/* check batt & bamo voltages */
		xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);

		// TODO: need to check units and determine acceptable bounds for voltages

		xSemaphoreTake(CAN_Inputs_Vector_Mutex);


		/* set SPI outputs */
		xSemaphoreTake(SPI_Outputs_Vector_Mutex, portMAX_DELAY);

		// set safety-out pin
		SPI_outputs_vector.safety = 1;
		SPI_outputs_vector.ready_to_drive = 0;
		SPI_outputs_vector.rfg = 0;

		xSemaphoreGive(SPI_Outputs_Vector_Mutex);


		/* wait for 90% precharge */
		xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);

		float pack_voltage = (float) CAN_inputs[BATT_VOLTAGE]/100.0f;
		float bus_voltage;

		xSemaphoreTake(CAN_Inputs_Vector_Mutex);
		// set precharge timer & wait
		// send Bamocar set message
		// alert driver AIRs are closed
		break;
	case READY_TO_DRIVE:
		// wait for push button
		// start buzzer timer
		break;
	case DRIVE:
		// set rfg output pin
		break;
	case FAULT:
		break;
	}
}
