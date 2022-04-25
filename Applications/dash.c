/*
 * dash.c
 *
 *  Created on: Apr 21, 2022
 *      Author: Andrei Gerashchenko
 */
#include "dash.h"
#include "cvc_can.h"

// CVC state and battery info
queue_msg_t dash_msg_1 =
{
	.Tx_header = {DASH_MSG_1_ID, 0, STD, RTR, 0x8 },
	.data._8 = {0, 0, 0, 0, 0, 0, 0, 0}
};

queue_msg_t dash_msg_2 =
{
		.Tx_header = {DASH_MSG_2_ID, 0, STD, RTR, 0x8},
		.data._8 = {0, 0, 0, 0 ,0 ,0 ,0, 0}
};


/*
 * Sends information about the car's current state to the dashboard
 */
void dash_update() {
	dash_msg_1.data._8[0] = (uint8_t)cvc_state;
	dash_msg_1.data._8[1] = (uint8_t)cvc_fault;
	// TODO: Pack state and fault into one byte
	dash_msg_1.data._8[2] = (uint8_t)((CAN_inputs[BATT_VOLTAGE] & 0xFF000000) >> 24);
	dash_msg_1.data._8[3] = (uint8_t)((CAN_inputs[BATT_VOLTAGE] & 0x00FF0000) >> 16);
	dash_msg_1.data._8[4] = (uint8_t)((CAN_inputs[BATT_VOLTAGE] & 0x0000FF00) >> 8);
	dash_msg_1.data._8[5] = (uint8_t)(CAN_inputs[BATT_VOLTAGE] & 0x000000FF);
	dash_msg_1.data._8[6] = (uint8_t)((CAN_inputs[BATT_CURRENT] & 0xFF00) >> 8);
	dash_msg_1.data._8[7] = (uint8_t)(CAN_inputs[BATT_CURRENT] & 0x00FF);

	CAN_Send(dash_msg_1);
}

