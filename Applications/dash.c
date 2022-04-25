/*
 * dash.c
 *
 *  Created on: Apr 21, 2022
 *      Author: Andrei Gerashchenko
 */
#include "dash.h"
#include "cvc_can.h"

queue_msg_t dash_msg_1 =
{
	.Tx_header = {DASH_MSG_1_ID, 0, STD, RTR, 0x8 },
	.data._8 = {0, 0, 0, 0, 0, 0, 0, 0}
};

void dash_update() {

	CAN_Send(dash_msg_1);
}

