/*
 * pm100.c
 *
 *  Created on: Oct 26, 2019
 *      Author: Benjamin Newhall
 */

#include "cvc_can.h"
#include "pm100.h"


/* pm100 Command Message*/
queue_msg_t pm100_command_msg_1 =
{
	.Tx_header = {CAN_ID_OFFSET1 + 0x020, 0, STD, RTR, 0x8},
	.data._8 = {0, 0, 0, 0, 0, 0, 0, 0}
};

queue_msg_t pm100_command_msg_2 =
{
	.Tx_header = {CAN_ID_OFFSET2 + 0x020, 0, STD, RTR, 0x8},
	.data._8 = {0, 0, 0, 0, 0, 0, 0, 0}
};
