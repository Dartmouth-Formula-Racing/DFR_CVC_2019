/*
 * cvc_logging.h
 *
 *  Created on: Feb 6, 2019
 *      Author: f002bc7
 */

#ifndef INC_CVC_LOGGING_H_
#define INC_CVC_LOGGING_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx_nucleo_144.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ff_test.h"
#include "cvc_can.h"
#include "diskio.h"
#include "ff_gen_drv.h"
#include "sd_diskio_dma.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Function Declarations -----------------------------------------------------*/
void logging_init();
void log_data(void);


#endif /* INC_CVC_LOGGING_H_ */
