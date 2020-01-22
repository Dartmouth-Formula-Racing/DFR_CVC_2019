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


#define OFFSET_MAX (16384.0f)
#define LOWER_TPS_LIMIT (16.0f)
#define UPPER_TPS_LIMIT (165.0f)
#define MULTIPLIER (1.0f)


void torque_command(void);

#endif /* INC_TORQUE_COMMAND_H_ */
