/*
 * bamocar.h
 *
 *  Created on: Feb 13, 2019
 *      Author: f002bc7
 */

#ifndef INC_BAMOCAR_H_
#define INC_BAMOCAR_H_

/* Definitions for Bamocar CAN Messages */
#define TRANSMIT_TO_BAMO_ID 		0x500 // 0x210
#define BAMO_READ_REGID			 	0x3D
#define BAMO_100_ms_transmission 	0x64 // 0x64 = 100

#define MOTOR_RPM_REGID 			0x30
#define MOTOR_CURRENT_REGID			0x5F
#define MOTOR_TORQUE_REGID 			0xA0
#define MOTOR_VOLTAGE_REGID 		0x8A
#define MOTOR_TEMP_REGID 			0x49
#define BAMOCAR_FAULT_REGID 		0x8F
#define BAMOCAR_BUS_VOLTAGE_REGID 	0xEB
#define BAMOCAR_D_OUT_1_REGID 		0xE0
#define BAMOCAR_D_OUT_2_REGID 		0xE1
#define HMC_STD_ID 					0x7FF
#define BAMOCAR_VAR1 				0xD1
#define VAR1_SET_VALUE 				0x01
#define VAR1_RESET_VALUE 			0x00



#endif /* INC_BAMOCAR_H_ */
