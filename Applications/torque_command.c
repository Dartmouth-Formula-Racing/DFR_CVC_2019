/*
 * torque_command.c
 *
 *  Created on: Feb 16, 2019
 *      Author: f002ccx
 */


#include "torque_command.h"


uint8_t ICE_tps = 0;
float calc_torque = 0.0f;
uint16_t torque_cmd = 0;
float tps_percentage = 0.0f;

void torque_command(void)
{
	ICE_tps = CAN_inputs[ENG_TPS];

	tps_percentage = (ICE_tps - LOWER_TPS_LIMIT)/(UPPER_TPS_LIMIT - LOWER_TPS_LIMIT);

	if (tps_percentage >= 1)
	{
		tps_percentage = 1;
	}
	else if (tps_percentage <= 0)
	{
		tps_percentage = 0;
	}

	calc_torque = tps_percentage*OFFSET_MAX*MULTIPLIER;

	torque_cmd = calc_torque;

	bamo_torque_command(torque_cmd);

}
