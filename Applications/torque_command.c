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
int errcount = 0;

queue_msg_t pm100_command_msg_1_aa =
{
	.Tx_header = {0x0AA, 0, STD, RTR, 0x8},
	.data._8 = {0, 0, 0, 0, 0, 0, 0, 0}
};

queue_msg_t pm100_command_msg_1_c0 =
{
	.Tx_header = {0x0C0, 0, STD, RTR, 0x8},
	.data._8 = {0, 0, 0, 0, 0, 0, 0, 0}
};



int count = 0;


void torque_command()
{
		if (get_cvc_state()== DRIVE) {

	if (count < 1000) {
		pm100_command_msg_1_aa.data._8[0] = 4;
		pm100_command_msg_1_aa.data._8[1] = 0;
		pm100_command_msg_1_aa.data._8[2] = 9;
		pm100_command_msg_1_aa.data._8[3] = 0;
		pm100_command_msg_1_aa.data._8[4] = 0;
		pm100_command_msg_1_aa.data._8[5] = 0;
		pm100_command_msg_1_aa.data._8[6] = 128;
		pm100_command_msg_1_aa.data._8[7] = 0;
		CAN_Send(pm100_command_msg_1_aa);
	} else if (count < 1500) {
		pm100_command_msg_1_c0.data._8[0] = 0;
		pm100_command_msg_1_c0.data._8[1] = 0;
		pm100_command_msg_1_c0.data._8[2] = 0;
		pm100_command_msg_1_c0.data._8[3] = 0;
		pm100_command_msg_1_c0.data._8[4] = 0;
		pm100_command_msg_1_c0.data._8[5] = 0;
		pm100_command_msg_1_c0.data._8[6] = 0;
		pm100_command_msg_1_c0.data._8[7] = 0;
		CAN_Send(pm100_command_msg_1_c0);
	} else if (count < 2000) {
		pm100_command_msg_1_c0.data._8[0] = 40;
		pm100_command_msg_1_c0.data._8[1] = 0;
		pm100_command_msg_1_c0.data._8[2] = 0;
		pm100_command_msg_1_c0.data._8[3] = 0;
		pm100_command_msg_1_c0.data._8[4] = 1;
		pm100_command_msg_1_c0.data._8[5] = 1;
		pm100_command_msg_1_c0.data._8[6] = 0;
		pm100_command_msg_1_c0.data._8[7] = 0;
		CAN_Send(pm100_command_msg_1_c0);
	} else if (count < 2500) {
		pm100_command_msg_1_c0.data._8[0] = 0;
		pm100_command_msg_1_c0.data._8[1] = 0;
		pm100_command_msg_1_c0.data._8[2] = 0;
		pm100_command_msg_1_c0.data._8[3] = 0;
		pm100_command_msg_1_c0.data._8[4] = 1;
		pm100_command_msg_1_c0.data._8[5] = 1;
		pm100_command_msg_1_c0.data._8[6] = 0;
		pm100_command_msg_1_c0.data._8[7] = 0;
		CAN_Send(pm100_command_msg_1_c0);
	} else if (count < 3000) {
		pm100_command_msg_1_c0.data._8[0] = 40;
		pm100_command_msg_1_c0.data._8[1] = 0;
		pm100_command_msg_1_c0.data._8[2] = 0;
		pm100_command_msg_1_c0.data._8[3] = 0;
		pm100_command_msg_1_c0.data._8[4] = 1;
		pm100_command_msg_1_c0.data._8[5] = 1;
		pm100_command_msg_1_c0.data._8[6] = 0;
		pm100_command_msg_1_c0.data._8[7] = 0;
		CAN_Send(pm100_command_msg_1_c0);
	} else if (count < 3500) {
		pm100_command_msg_1_c0.data._8[0] = 100;
		pm100_command_msg_1_c0.data._8[1] = 255;
		pm100_command_msg_1_c0.data._8[2] = 0;
		pm100_command_msg_1_c0.data._8[3] = 0;
		pm100_command_msg_1_c0.data._8[4] = 1;
		pm100_command_msg_1_c0.data._8[5] = 0;
		pm100_command_msg_1_c0.data._8[6] = 0;
		pm100_command_msg_1_c0.data._8[7] = 0;
		CAN_Send(pm100_command_msg_1_c0);
	} else if (count < 4000) {
		pm100_command_msg_1_c0.data._8[0] = 0;
		pm100_command_msg_1_c0.data._8[1] = 0;
		pm100_command_msg_1_c0.data._8[2] = 0;
		pm100_command_msg_1_c0.data._8[3] = 0;
		pm100_command_msg_1_c0.data._8[4] = 0;
		pm100_command_msg_1_c0.data._8[5] = 1;
		pm100_command_msg_1_c0.data._8[6] = 0;
		pm100_command_msg_1_c0.data._8[7] = 0;
		CAN_Send(pm100_command_msg_1_c0);
	}
	count = count + 1; }
//	if (get_cvc_state()== DRIVE) {
//		// Read throttle pots
//		process_throttle();
//		// Convert torque c`ommand value
//
//
////		 TODO: get drive state only once for efficiency?
//		if (get_drive_state() == DRIVE) {
//			torque_linear(TORQUE_MULTIPLIER_FORWARD);
//			pm100_torque_command_1(torque_int(calc_torque_1), 0);
//			pm100_torque_command_2(torque_int(calc_torque_2), 1);
//
//		} else if (get_drive_state() == NEUTRAL) {
//			command_msg_1(0,0,0,0,0,0,0);
//			command_msg_2(0,0,0,0,0,0,0);
//		} else if (get_drive_state() == REVERSE) {
//			torque_linear(TORQUE_MULTIPLIER_REVERSE);
//			pm100_torque_command_1(torque_int(calc_torque_1), 1);
//			pm100_torque_command_2(torque_int(calc_torque_2), 0);
//		} else {
//			command_msg_1(0,0,0,0,0,0,0);
//			command_msg_2(0,0,0,0,0,0,0);
//		}
//	}
//	else {
//		command_msg_1(0,0,0,0,0,0,0);
//		command_msg_2(0,0,0,0,0,0,0);
//	}
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
	uint32_t throttle_2 = clamp_throttle(CAN_inputs[ANALOG_INPUT_6_2], THROTTLE_ZERO_2, THROTTLE_ONE_2);

	float tps_1 = ((float)(throttle_1 - THROTTLE_ZERO_1))/((float)(THROTTLE_ONE_1 - THROTTLE_ZERO_1));
	float tps_2 = ((float)(throttle_2 - THROTTLE_ZERO_2))/((float)(THROTTLE_ONE_2 - THROTTLE_ZERO_2));

	tps_percentage = (tps_1 + tps_2)/2;

	if ((tps_1 - tps_2) > MAX_THROTTLE_ERROR || (tps_2 - tps_1) > MAX_THROTTLE_ERROR)
	{
		errcount++;
	}
	else
	{
		errcount = 0;
	}
	if (errcount > MAX_ERROR_COUNT)
	{
		init_fault_handler();
		tps_percentage = 0;
	}


	xSemaphoreGive(CAN_Inputs_Vector_Mutex);
}

uint32_t clamp_throttle(uint32_t d, uint32_t min, uint32_t max) {
  const uint32_t t = d < min ? min : d;
  return t > max ? max : t;
}

uint16_t torque_int(float torque)
{
	int16_t signed_torque = (int16_t)(torque * 10.0f);
	return signed_torque;
}

void torque_linear(float multiplier)
{
	calc_torque_1 = (tps_percentage-0.5) * UPPER_TORQUE_LIMIT * multiplier;
	calc_torque_2 = (tps_percentage-0.5) * UPPER_TORQUE_LIMIT * multiplier;
}

//int16_t xBuffer = (CAN_inputs[X_AXIS_ACCELERATION]);
//int16_t yBuffer = (CAN_inputs[Y_AXIS_ACCELERATION]);
//int16_t zBuffer = (CAN_inputs[Z_AXIS_ACCELERATION]);
