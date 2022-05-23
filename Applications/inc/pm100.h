/*
 * pm100.h
 *
 *  Created on: Oct 26, 2019
 *      Author: Benjamin Newhall
 */

#ifndef PM_100_H
#define PM_100_H


//Defines
#define CAN_ID_OFFSET1 0xA0
#define CAN_ID_OFFSET2 0xD0 //TODO: fix Offset for pm100 controllers

void pm100_torque_command_1(uint16_t torque_command, uint8_t direction_command);

void pm100_torque_command_2(uint16_t torque_command, uint8_t direction_command);


#endif
