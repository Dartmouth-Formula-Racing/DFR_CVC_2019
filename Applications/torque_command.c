/*
 * torque_command.c
 *
 *  Created on: Feb 16, 2019
 *      Author: f002ccx
 */


#include "torque_command.h"
#include "cvc_spi.h"
#include "cvc_state_machine.h"
#include "pm100.h"

#define TESTMODE 0

float calc_torque_1 = 0.0f;
float calc_torque_2 = 0.0f;
uint16_t torque_cmd_1 = 0;
uint16_t torque_cmd_2 = 0;
float tps_percentage = 0.0f;

void torque_command()
{
	if (get_cvc_state() == DRIVE && !CAN_inputs[INVERTER_ENABLE_LOCKOUT]) {
		// Read throttle pots
		process_throttle();
		// Convert torque command value


		// TODO: get drive state only once for efficiency?
		if (get_drive_state() == DRIVE) {
			torque_linear(TORQUE_MULTIPLIER_FORWARD);
			pm100_torque_command_1(torque_int(calc_torque_1), 1);
			pm100_torque_command_2(torque_int(calc_torque_2), 1);
		} else if (get_drive_state() == NEUTRAL) {
			command_msg_1(0,0,0,0,0,0,0);
			command_msg_2(0,0,0,0,0,0,0);
		} else if (get_drive_state() == REVERSE) {
			torque_linear(TORQUE_MULTIPLIER_REVERSE);
			pm100_torque_command_1(torque_int(calc_torque_1), 0);
			pm100_torque_command_2(torque_int(calc_torque_2), 0);
		} else {
			command_msg_1(0,0,0,0,0,0,0);
			command_msg_2(0,0,0,0,0,0,0);
		}
	}
}

int32_t intmap(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float floatmap(float x, float in_min, float in_max, float out_min, float out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void process_throttle()
{
	xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);

	uint32_t throttle_1 = clamp_throttle(CAN_inputs[ANALOG_INPUT_6], THROTTLE_ZERO_1, THROTTLE_ONE_1);
	uint32_t throttle_2 = clamp_throttle(CAN_inputs[ANALOG_INPUT_6_2], THROTTLE_ONE_2, THROTTLE_ZERO_2);

	float tps_1 = ((float)(throttle_1 - THROTTLE_ZERO_1))/((float)(THROTTLE_ONE_1 - THROTTLE_ZERO_1));
	float tps_2 = ((float)(THROTTLE_ZERO_2 - throttle_2))/((float)(THROTTLE_ZERO_2 - THROTTLE_ONE_2));

	tps_percentage = (tps_1 + tps_2)/2;

	xSemaphoreGive(CAN_Inputs_Vector_Mutex);
}

uint32_t clamp_throttle(uint32_t d, uint32_t min, uint32_t max) {
  const uint32_t t = d < min ? min : d;
  return t > max ? max : t;
}

uint16_t torque_int(float torque)
{
	int16_t signed_torque = (int16_t)(torque * 10.0f);
	return (uint16_t)signed_torque;
}

void torque_linear(float multiplier)
{
	calc_torque_1 = tps_percentage * UPPER_TORQUE_LIMIT * multiplier;
	calc_torque_2 = tps_percentage * UPPER_TORQUE_LIMIT * multiplier;
}

//int16_t xBuffer = (CAN_inputs[X_AXIS_ACCELERATION]);
//int16_t yBuffer = (CAN_inputs[Y_AXIS_ACCELERATION]);
//int16_t zBuffer = (CAN_inputs[Z_AXIS_ACCELERATION]);
