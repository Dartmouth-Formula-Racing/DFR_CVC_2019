/*
 * demo.h
 *
 *  Created on: Dec 1, 2018
 *      Author: f002bc7
 */

#ifndef INC_DEMO_H_
#define INC_DEMO_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx_nucleo_144.h"
#include "FreeRTOS.h"
#include "task.h"

/* Function Declarations -----------------------------------------------------*/
void demoTask(void * parameters);

#endif /* INC_DEMO_H_ */
