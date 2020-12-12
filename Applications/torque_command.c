/*
 * torque_command.c
 *
 *  Created on: Feb 16, 2019
 *      Author: f002ccx
 */

#include "torque_command.h"


uint8_t ICE_tps = 0;
float calc_torque = 0.0f;
uint16_t torque_cmd = 0;
float tps_percentage = 0.0f;

/** 
 * @brief a way to switch to different drive modes
 * @param mode the mode at which two characters are taken as inputs and sends to car
*/
void torque_command(char mode[2])
{
    if(HAL_GPIO_ReadPin(B1_GPIO_PORT, B1_PIN)&& CAN_inputs[INVERTER_ENABLE_LOCKOUT] == 0)
    {
		switch (mode)
        {
            case 'LA':                   // linear acceleration mode
                calc_torque = linear_acceleration();
                break;
                
            default:
                break;
        }
    } else {
        calc_torque = 0;
    }

    command_msg_1(calc_torque, 0, 1, 1, 0, 0, 0);       // send to rinehart 1
    command_msg_2(calc_torque, 0, 1, 1, 0, 0, 0);       // send to rinehart 2

}

/**
 * @brief the way to calculate torque for the car acceleration in a straight line
 */
double linear_acceleration(void)
{
    tps = CAN_inputs[ENG_TPS];

	tps_percentage = ((tps - LOWER_TPS_LIMIT)/(UPPER_TPS_LIMIT - LOWER_TPS_LIMIT) * 100) ** 2;      // can test with other growing functions e.g e^x

	if (tps_percentage >= 100)
	{
		tps_percentage = 100;
	}
	else if (tps_percentage <= 0)
	{
		tps_percentage = 0;
	}

	calc_torque = (tps_percentage * OFFSET_MAX * MULTIPLIER) / 100;
    
    return calc_torque;
}