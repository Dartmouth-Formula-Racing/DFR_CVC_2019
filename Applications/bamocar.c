/*
 * bamocar.c
 *
 *  Created on: Feb 13, 2019
 *      Author: f002bc7
 */

#include "bamocar.h"
#include "cvc_can.h"


#define	INIT_MESSAGE_COUNT 10

/* Bamocar initialize CAN messages */
queue_msg_t bamocar_init_msg[INIT_MESSAGE_COUNT] =
{
	{
		.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR, 0x3 },													// (0)
		.data._8 = {BAMO_READ_REGID, MOTOR_RPM_REGID, BAMO_100_ms_transmission, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR, 0x3	},													// (1)
		.data._8 = {BAMO_READ_REGID, MOTOR_CURRENT_REGID, BAMO_100_ms_transmission, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR,	0x3 },													// (2)
		.data._8 = {BAMO_READ_REGID, MOTOR_TORQUE_REGID, BAMO_100_ms_transmission, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR, 0x3 },													// (3)
		.data._8 = {BAMO_READ_REGID, MOTOR_VOLTAGE_REGID, BAMO_100_ms_transmission, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR, 0x3 },													// (4)
		.data._8 = {BAMO_READ_REGID, MOTOR_TEMP_REGID, BAMO_100_ms_transmission, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR,	0x3 },													// (5)
		.data._8 = {BAMO_READ_REGID, BAMOCAR_FAULT_REGID, BAMO_100_ms_transmission, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR, 0x3 },													// (6)
		.data._8 = {BAMO_READ_REGID, BAMOCAR_BUS_VOLTAGE_REGID, BAMO_100_ms_transmission, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR,	0x3 },													// (7)
		.data._8 = {BAMO_READ_REGID, BAMOCAR_D_OUT_1_REGID, BAMO_100_ms_transmission, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR,	0x3 },													// (8)
		.data._8 = {BAMO_READ_REGID, BAMOCAR_D_OUT_2_REGID, BAMO_100_ms_transmission, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
		.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR, 0x5 },													// (9)
		.data._8 = {BAMOCAR_VAR1, VAR1_RESET_VALUE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
};

/* Bamocar VAR1 RESET CAN message */
queue_msg_t bamocar_var1_reset_msg =
{
	.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR, 0x3 },
	.data._8 = {BAMOCAR_VAR1,    VAR1_RESET_VALUE, 0 , 0, 0, 0, 0, 0}
};

/* Bamocar VAR1 SET CAN message */
queue_msg_t bamocar_var1_msg =
{
	.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR, 0x3 },
	.data._8 = {BAMOCAR_VAR1,    VAR1_SET_VALUE, 0 , 0, 0, 0, 0, 0}
};

/* Bamocar Torque Command CAN message */
queue_msg_t bamocar_torque_msg =
{
		.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR, 0x5},
		.data._8 = {MOTOR_TORQUE_COMMAND_REGID, 0, 0, 00, 0x10, 0, 0, 0}
};

/**
 * @brief: send next Bamocar init message until all have been sent
 * @ret: 1 if init complete, 0 if incomplete
 */
uint8_t init_index = 0;
uint8_t bamo_init(void)
{
	if (init_index < INIT_MESSAGE_COUNT)
	{
		CAN_Send(bamocar_init_msg[init_index]);		/* send init message */
		init_index++;
		return 0;		/* return incomplete status */
	}
	else
	{
		init_index = 0;
		return 1;		/* return complete status */
	}
}

void bamo_var1_reset(void)
{
	CAN_Send(bamocar_var1_reset_msg);
}

void bamo_var1_set(void)
{
	CAN_Send(bamocar_var1_msg);
}


void bamo_torque_command(uint16_t torque_command)
{

	bamocar_torque_msg.data._8[1] = (torque_command & 0x00FF);
	bamocar_torque_msg.data._8[2] = ((torque_command & 0xFF00) >> 8);

	CAN_Send(bamocar_torque_msg);
}
