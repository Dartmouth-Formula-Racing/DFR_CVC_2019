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

	if(HAL_GPIO_ReadPin(B1_GPIO_PORT, B1_PIN)&& CAN_inputs[INVERTER_ENABLE_LOCKOUT] == 0){
		command_msg_1(50, 0, 1, 1, 0, 0, 0);
	}
	else{
		command_msg_1(0,0,0,0,0,0,0);
	}

}
