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

uint8_t battery_voltage = 0;
uint8_t engine_rpm = 0;

float tps_percentage = 0.0f;
float decay_val = 0.0f;
int count = 0;

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

	battery_voltage = CAN_inputs[BATT_VOLTAGE]/100;
	decay_val = (battery_voltage-MIN_BATTERY_VOLTAGE)/(MAX_BATTERY_VOLTAGE-MIN_BATTERY_VOLTAGE);


	engine_rpm = CAN_inputs[ENG_RPM];

	if (engine_rpm == 0)
	{
		count++;

		if (count > RPM_DEAD_COUNT)
		{
			calc_torque = tps_percentage*OFFSET_MAX*MULTIPLIER;
		}
	}
	else
	{
		count = 0;
		calc_torque = decay_val*tps_percentage*OFFSET_MAX*MULTIPLIER;
	}


	torque_cmd = calc_torque;

	bamo_torque_command(torque_cmd);

}
