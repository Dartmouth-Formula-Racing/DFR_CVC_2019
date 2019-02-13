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
		// send Bamocar init messages
		break;
	case PRECHARGE:
		// check batt & bamo voltages
		// set safety-out pin
		// wait for 90% precharge
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
