/*
 * torque_command.c
 *
 *  Created on: Feb 16, 2019
 *      Author: f002ccx
 */


#include "torque_command.h"
#include "cvc_spi.h"
#include "cvc_state_machine.h"

#define TESTMODE 0

uint8_t ICE_tps = 0;
float calc_torque = 0.0f;
uint16_t torque_cmd = 0;
float tps_percentage = 0.0f;

void torque_command()
{
	if (get_cvc_state() == DRIVE && !CAN_inputs[INVERTER_ENABLE_LOCKOUT]) {
		// Linear tps to torque
		tps_percentage = floatmap(16.0, LOWER_TPS_LIMIT, UPPER_TPS_LIMIT, 0.0, 100.0);
		// Add 0.5 and cast as int to round
		torque_cmd = (uint16_t)(floatmap(tps_percentage, 0.0, 100.0, LOWER_TORQUE_LIMIT, UPPER_TORQUE_LIMIT) + 0.5);


		if (TESTMODE) {
			// Motor spinup test mode (hardcoded torque)
			if(HAL_GPIO_ReadPin(B1_GPIO_PORT, B1_PIN)){
				command_msg_1(200, 0, 1, 1, 0, 0, 0);
			} else {
				command_msg_1(0,0,0,0,0,0,0);
			}

		} else if (get_drive_state() == DRIVE) {
			command_msg_1(torque_cmd, 0, 1, 1, 0, 0, 0);
			command_msg_2(torque_cmd, 0, 1, 1, 0, 0, 0);
		} else if (get_drive_state() == NEUTRAL) {
			command_msg_1(0, 0, 1, 1, 0, 0, 0);
			command_msg_2(0, 0, 1, 1, 0, 0, 0);
		} else if (get_drive_state() == REVERSE) {
			command_msg_1(torque_cmd, 0, 0, 1, 0, 0, 0);
			command_msg_2(torque_cmd, 0, 0, 1, 0, 0, 0);
		} else {
			command_msg_1(0,0,0,0,0,0,0);
			command_msg_2(0,0,0,0,0,0,0);
		}
	}
}

uint32_t intmap(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float floatmap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
