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

#define MIN_TPS_THRESHOLD  		0
#define MAX_TPS_THRESHOLD		250
#define TPS_FAULT_LIMIT			5

#define COOLANT_TIMER_LOAD	10		// 0.1 seconds, @ 100 Hz = 10 ticks



void monitor_temps(void);
void monitor_engine(void);
void monitor_tps(void);

#endif /* INC_ICE_H_ */
