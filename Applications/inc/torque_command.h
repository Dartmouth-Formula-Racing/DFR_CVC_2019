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
#define LOWER_TPS_LIMIT (3.0f)
#define UPPER_TPS_LIMIT (232.0f)
#define MULTIPLIER (0.15f)


void torque_command(void);

#endif /* INC_TORQUE_COMMAND_H_ */
