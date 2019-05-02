/*
 * torque_command.h
 *
 *  Created on: Feb 16, 2019
 *      Author: f002ccx
 */

#ifndef INC_TORQUE_COMMAND_H_
#define INC_TORQUE_COMMAND_H_

#include <stdint.h>
#include "cvc_can.h"
#include "bamocar.h"

#define OFFSET_MAX (16384.0f)
#define LOWER_TPS_LIMIT (16.0f)
#define UPPER_TPS_LIMIT (202.0f)
#define MULTIPLIER (1.0f)

#define MIN_BATTERY_VOLTAGE (230.0f)
#define MAX_BATTERY_VOLTAGE (300.0f)

#define RPM_DEAD_COUNT 100

void torque_command(void);

#endif /* INC_TORQUE_COMMAND_H_ */
