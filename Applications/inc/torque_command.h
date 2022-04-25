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
#define UPPER_TORQUE_LIMIT (900.0f)
#define LOWER_TORQUE_LIMIT (0.0f)
#define MULTIPLIER (1.0f)


uint32_t intmap(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max);

float floatmap(float x, float in_min, float in_max, float out_min, float out_max);

void torque_command(char* mode, float tps, uint8_t direction);

#endif /* INC_TORQUE_COMMAND_H_ */
