/*
 * cvc_logging.h
 *
 *  Created on: Feb 6, 2019
 *      Author: f002bc7
 */

#ifndef INC_CVC_LOGGING_H_
#define INC_CVC_LOGGING_H_

/* Includes ------------------------------------------------------------------*/

/* Standard Includes */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* STM Includes */
#include "stm32f7xx_hal.h"
#include "stm32f7xx_nucleo_144.h"

/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"

/* FatFS Includes */
#include "ff_test.h"
#include "diskio.h"
#include "ff_gen_drv.h"
#include "sd_diskio_dma.h"

/* CVC Includes */
#include "cvc_can.h"
#include "cvc_state_machine.h"


/* Defines ------------------------------------------------------------------*/

#define LOGGING_TEST		1

/* Global variables ------------------------------------------------------------------*/
extern uint8_t LOG_FLAG;

/* Function Declarations -----------------------------------------------------*/
int logging_init();
void log_data(void);


#endif /* INC_CVC_LOGGING_H_ */
