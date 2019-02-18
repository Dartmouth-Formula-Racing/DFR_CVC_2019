/*
 * ice.h
 *
 *  Created on: Feb 17, 2019
 *      Author: f002ccx
 */

#ifndef INC_ICE_H_
#define INC_ICE_H_

#include <stdint.h>

#define MAX_COOLANT_TEMP	90
#define MAX_OIL_TEMP		100


void monitor_temps(void);
void monitor_engine(void);

#endif /* INC_ICE_H_ */
