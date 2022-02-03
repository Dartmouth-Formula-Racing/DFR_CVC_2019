/*
 * torque_command.c
 *
 *  Created on: Feb 16, 2019
 *      Author: f002ccx
 */

#include "torque_command.h"
#include "pm100.h"

uint8_t ICE_tps = 0;
uint16_t calc_torque = 0;
uint16_t torque_cmd = 0;
float tps_percentage = 0.0f;

/** 
 * @brief a way to switch to different drive modes
 * @param mode the mode at which two characters are taken as inputs and sends to car
*/
void torque_command(char mode[2])
{
	switch (mode)
    {
        case 'LA':                   // linear acceleration mode
            calc_torque = linear_acceleration();
            break;
            
        default:
            calc_torque = 0;
            break;
    }

    pm100_torque_command_1(calc_torque, 1);
    pm100_torque_command_2(calc_torque, 1);

}

/**
 * @brief the way to calculate torque for the car acceleration in a straight line
 */
uint16_t linear_acceleration(void)
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

	calc_torque = (tps_percentage * OFFSET_MAX * MULTIPLIER) / 100 * 10;
    
    return calc_torque;
}