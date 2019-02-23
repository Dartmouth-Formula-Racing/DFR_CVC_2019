/*
 * synchronous.h
 *
 *  Created on: Dec 12, 2018
 *      Author: f002ccx
 */

#ifndef INC_SYNCHRONOUS_H_
#define INC_SYNCHRONOUS_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx_nucleo_144.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cvc_spi.h"
#include "semphr.h"
#include "cvc_state_machine.h"
#include "torque_command.h"
#include "cvc_logging.h"

/* Function Prototypes ------------------------------------------------ */
void _10_ms_Task(void * parameters);
void _50_ms_Task(void * parameters);


#endif /* INC_SYNCHRONOUS_H_ */
