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

queue_msg_t pm100_parameter_write_msg_1 =
{
	.Tx_header = {CAN_ID_OFFSET1 + 0x021, 0, STD, RTR, 0x8},
	.data._8 = {0, 0, 1, 0, 0, 0, 0, 0}
};

queue_msg_t pm100_parameter_read_msg_1 =
{
	.Tx_header = {CAN_ID_OFFSET1 + 0x021, 0, STD, RTR, 0x8},
	.data._8 = {0, 0, 0, 0, 0, 0, 0, 0}
};

queue_msg_t pm100_parameter_write_msg_2 =
{
	.Tx_header = {CAN_ID_OFFSET2 + 0x021, 0, STD, RTR, 0x8},
	.data._8 = {0, 0, 1, 0, 0, 0, 0, 0}
};

queue_msg_t pm100_parameter_read_msg_2 =
{
	.Tx_header = {CAN_ID_OFFSET2 + 0x021, 0, STD, RTR, 0x8},
	.data._8 = {0, 0, 0, 0, 0, 0, 0, 0}
};




/**
 * @brief a way to send parameter write messages to rinehart 1
 * @param parameter_address the Parameter Address for the message
 * @param data the data to send in bytes 4 and 5, should already be formatted in order [byte 4][byte 5] (formatting described in documentation)
 */
void parameter_write_command_1(uint16_t parameter_address, uint16_t data)
{

	pm100_parameter_write_msg_1.data._8[0] = (parameter_address & 0x00FF);
	pm100_parameter_write_msg_1.data._8[1] = ((parameter_address & 0xFF00) >> 8);

	pm100_parameter_write_msg_1.data._8[5] = ((data & 0xFF00) >> 8);
	pm100_parameter_write_msg_1.data._8[4] = (data & 0x00FF);
	CAN_Send(pm100_parameter_write_msg_1);
}

/**
 * @brief a way to send parameter write messages to rinehart 2
 * @param parameter_address the Parameter Address for the message
 * @param data the data to send in bytes 4 and 5, should already be formatted in order [byte 4][byte 5] (formatting described in documentation)
 */
void parameter_write_command_2(uint16_t parameter_address, uint16_t data)
{

	pm100_parameter_write_msg_2.data._8[0] = (parameter_address & 0x00FF);
	pm100_parameter_write_msg_2.data._8[1] = ((parameter_address & 0xFF00) >> 8);

	pm100_parameter_write_msg_2.data._8[5] = ((data & 0xFF00) >> 8);
	pm100_parameter_write_msg_2.data._8[4] = (data & 0x00FF);
	CAN_Send(pm100_parameter_write_msg_2);
}

/**
 * @brief a way to send parameter read messages to rinehart 1
 * @param parameter_address the Parameter Address for the message
 *
 */
void parameter_read_command_1(uint16_t parameter_address)
{

	pm100_parameter_read_msg_1.data._8[0] = (parameter_address & 0x00FF);
	pm100_parameter_read_msg_1.data._8[1] = ((parameter_address & 0xFF00) >> 8);


	CAN_Send(pm100_parameter_read_msg_1);
}

/**
 * @brief a way to send parameter read messages to rinehart 2
 * @param parameter_address the Parameter Address for the message
 */
void parameter_read_command_2(uint16_t parameter_address)
{

	pm100_parameter_read_msg_2.data._8[0] = (parameter_address & 0x00FF);
	pm100_parameter_read_msg_2.data._8[1] = ((parameter_address & 0xFF00) >> 8);


	CAN_Send(pm100_parameter_read_msg_2);
}
/**
 * @brief a way to send command messages to rinehart 1
 * @param torque_command the torque command to send in N*m times 10 (does parsing locally)
 * @param speed_command the angular speed to send in RPM
 * @param direction_command either one or zero (see documentation for use with brake regen)
 * @param inverter_enable either 1 or 0 (inverter on or off)
 * @param inverter_discharge either 1 or 0 (enable discharge or disable discharge)
 * @param speed_mode_enable 0= do not override mode 1= will change from torque to speed mode (see manual "using speed mode")
 * @param commanded_torque_limit the max torque limit in N*m times 10, if zero will default to parameter in EEPROM
 */
void command_msg_1(uint16_t torque_command, uint16_t speed_command, uint8_t direction_command, uint8_t inverter_enable, uint8_t inverter_discharge, uint8_t speed_mode_enable, uint16_t commanded_torque_limit){
	pm100_command_msg_1.data._8[0] = (torque_command & 0x00FF);
	pm100_command_msg_1.data._8[1] = ((torque_command & 0xFF00) >> 8);
	pm100_command_msg_1.data._8[2] = (speed_command & 0xFF00);
	pm100_command_msg_1.data._8[3] = ((speed_command & 0xFF00) >> 8);
	pm100_command_msg_1.data._8[4] = direction_command;
	pm100_command_msg_1.data._8[5] = inverter_enable;
	pm100_command_msg_1.data._8[5] |= inverter_discharge << 1;
	pm100_command_msg_1.data._8[5] |= speed_mode_enable << 2;
	pm100_command_msg_1.data._8[6] = (commanded_torque_limit & 0x00FF);
	pm100_command_msg_1.data._8[7] = ((commanded_torque_limit & 0xFF00) >> 8);

	CAN_Send(pm100_command_msg_1);
}


/**
 * @brief a way to send command messages to rinehart 1
 * @param torque_command the torque command to send in N*m times 10 (does parsing locally)
 * @param speed_command the angular speed to send in RPM
 * @param direction_command either one or zero (see documentation for use with brake regen)
 * @param inverter_enable either 1 or 0 (inverter on or off)
 * @param inverter_discharge either 1 or 0 (enable discharge or disable discharge)
 * @param speed_mode_enable 0= do not override mode 1= will change from torque to speed mode (see manual "using speed mode")
 * @param commanded_torque_limit the max torque limit in N*m times 10, if zero will default to parameter in EEPROM
 */
void command_msg_2(uint16_t torque_command, uint16_t speed_command, uint8_t direction_command, uint8_t inverter_enable, uint8_t inverter_discharge, uint8_t speed_mode_enable, uint16_t commanded_torque_limit){
	pm100_command_msg_2.data._8[0] = (torque_command & 0x00FF);
	pm100_command_msg_2.data._8[1] = ((torque_command & 0xFF00) >> 8);
	pm100_command_msg_2.data._8[2] = (speed_command & 0xFF00);
	pm100_command_msg_2.data._8[3] = ((speed_command & 0xFF00) >> 8);
	pm100_command_msg_2.data._8[4] = direction_command;
	pm100_command_msg_2.data._8[5] = inverter_enable;
	pm100_command_msg_2.data._8[5] |= inverter_discharge << 1;
	pm100_command_msg_2.data._8[5] |= speed_mode_enable << 2;
	pm100_command_msg_2.data._8[6] = (commanded_torque_limit & 0x00FF);
	pm100_command_msg_2.data._8[7] = ((commanded_torque_limit & 0xFF00) >> 8);

	CAN_Send(pm100_command_msg_2);
}

/* @brief a way to send torque messages to rinehart 1 (will disable lockout if lockout is enabled by sending empty message)
 * @param torque_command the torque command to send in N*m times 10 (does parsing locally)
 * @param direction_command either one or zero (see documentation for use with brake regen)
 */
void pm100_torque_command_1(uint16_t torque_command, uint8_t direction_command){
	xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);
	if(HAL_GPIO_ReadPin(B1_GPIO_PORT, B1_PIN)){
		command_msg_1(torque_command, 0, direction_command, 1, 0, 0, 0);
	}
	else{
		command_msg_1(0,0,0,0,0,0,0);
	}
	xSemaphoreGive(CAN_Inputs_Vector_Mutex);


}


/* @brief a way to send torque messages to rinehart 2 (will disable lockout if lockout is enabled by sending empty message)
 * @param torque_command the torque command to send in N*m times 10 (does parsing locally)
 * @param direction_command either one or zero (see documentation for use with brake regen)
 */
void pm100_torque_command_2(uint16_t torque_command, uint8_t direction_command){
	xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);
	if(CAN_inputs[INVERTER_ENABLE_LOCKOUT_2] == 1){
		command_msg_2(0,0,0,0,0,0,0);
	}
	else{
		command_msg_2(torque_command, 0, direction_command, 1, 0, 0, 0);
	}
	xSemaphoreGive(CAN_Inputs_Vector_Mutex);


}
