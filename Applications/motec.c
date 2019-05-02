/*
 * motec.c
 *
 *  Created on: Apr 30, 2019
 *      Author: f002ccx
 */

/************** Includes **************/
#include "motec.h"

queue_msg_t motec_msg_1 =
{
	.Tx_header = {MOTEC_MSG_1_ID, 0, STD, RTR, 0x8 },
	.data._8 = {0, 0, 0, 0, 0, 0, 0, 0}
};

queue_msg_t motec_msg_2 =
{
	.Tx_header = {MOTEC_MSG_2_ID, 0, STD, RTR, 0x2 },
	.data._8 = {0, 0, 0, 0, 0, 0, 0, 0}
};

void motec_update()
{
	/** MESSAGE 1 **/
	/* motor RPM */
	motec_msg_1.data._8[0] = (uint8_t) (CAN_inputs[MOTOR_RPM] >> 8);
	motec_msg_1.data._8[1] = (uint8_t) (CAN_inputs[MOTOR_RPM] & 0x000000FF);

	/* engine RPM  */
	motec_msg_1.data._8[2] = (uint8_t) (CAN_inputs[ENG_RPM] >> 24);
	motec_msg_1.data._8[3] = (uint8_t) (CAN_inputs[ENG_RPM] & 0x000000FF);

	/* pack voltage */
	motec_msg_1.data._8[4] = (uint8_t) (CAN_inputs[BATT_VOLTAGE] >> 24);
	motec_msg_1.data._8[5] = (uint8_t) (CAN_inputs[BATT_VOLTAGE] >> 16 & 0x000000FF);
	motec_msg_1.data._8[6] = (uint8_t) (CAN_inputs[BATT_VOLTAGE] >> 8 & 0x000000FF);
	motec_msg_1.data._8[7] = (uint8_t) (CAN_inputs[BATT_VOLTAGE] & 0x000000FF);

	CAN_Send(motec_msg_1);

	/** MESSAGE 2 **/
	/* TODO: add fuel level */

}
