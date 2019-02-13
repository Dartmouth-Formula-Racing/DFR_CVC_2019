/*
 * bamocar.c
 *
 *  Created on: Feb 13, 2019
 *      Author: f002bc7
 */

#include "bamocar.h"
#include "cvc_can.h"

queue_msg_t bamocar_init_msg[] =
{
	{
			.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR, 0x3 },													// (0)
			.data = {BAMO_READ_REGID, MOTOR_RPM_REGID, BAMO_100_ms_transmission, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
			.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR, 0x3	},													// (1)
			.data = {BAMO_READ_REGID, MOTOR_CURRENT_REGID, BAMO_100_ms_transmission, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
			.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR,	0x3 },													// (2)
			.data = {BAMO_READ_REGID, MOTOR_TORQUE_REGID, BAMO_100_ms_transmission, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
			.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR, 0x3 },													// (3)
			.data = {BAMO_READ_REGID, MOTOR_VOLTAGE_REGID, BAMO_100_ms_transmission, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
			.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR, 0x3 },													// (4)
			.data = {BAMO_READ_REGID, MOTOR_TEMP_REGID, BAMO_100_ms_transmission, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
			.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR,	0x3 },													// (5)
			.data = {BAMO_READ_REGID, BAMOCAR_FAULT_REGID, BAMO_100_ms_transmission, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
			.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR, 0x3 },													// (6)
			.data = {BAMO_READ_REGID, BAMOCAR_BUS_VOLTAGE_REGID, BAMO_100_ms_transmission, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
			.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR,	0x3 },													// (7)
			.data = {BAMO_READ_REGID, BAMOCAR_D_OUT_1_REGID, BAMO_100_ms_transmission, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
			.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR,	0x3 },													// (8)
			.data = {BAMO_READ_REGID, BAMOCAR_D_OUT_2_REGID, BAMO_100_ms_transmission, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
	{
			.Tx_header = {TRANSMIT_TO_BAMO_ID, 0, STD, RTR, 0x5 },													// (9)
			.data = {BAMOCAR_VAR1, VAR1_RESET_VALUE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
	},
};
