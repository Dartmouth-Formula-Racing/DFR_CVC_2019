/*
 * ice.c
 *
 *  Created on: Feb 17, 2019
 *      Author: f002ccx
 */

#include "ice.h"
#include "cvc_can.h"
#include "cvc_state_machine.h"

uint32_t coolant_temp;
uint32_t air_temp;
uint32_t oil_temp;

void monitor_engine(void)
{
	monitor_temps();
}


void monitor_temps(void)
{
	coolant_temp = CAN_inputs[ENG_TEMP]-40;

	if (coolant_temp > MAX_COOLANT_TEMP)
	{
		error_handler(CVC_HARD_FAULT, ENGINE_ERR);
	}


	air_temp = CAN_inputs[AIR_TEMP]-40;

	oil_temp = CAN_inputs[OIL_TEMP]-40;

	/* TODO: check oil temperature sensing on engine */
//	oil_temp = CAN_inputs[OIL_TEMP]-40;
//
//	if (oil_temp > MAX_OIL_TEMP)
//	{
//		error_handler(CVC_HARD_FAULT, ENGINE_ERR);
//	}
}
