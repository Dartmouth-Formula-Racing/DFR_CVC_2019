/*
 * cvc_sara.h
 *
 *  Created on: Jan 18, 2020
 *      Author: f003kgj
 */

#ifndef INC_CVC_SARA_H_
#define INC_CVC_SARA_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal_uart.h"

//#include "FreeRTOS.h"

/* Defines -------------------------------------------------------------------*/
#define SARA_UART_BAUD_RATE			115200 // default baud for sara-r4 modem

/* Typedefs ----------------------------------------------------------*/


/* External Variables ------------------------------------------------------------*/
extern volatile UART_HandleTypeDef huart;

/* Function prototypes -----------------------------------------------*/


#endif /* INC_CVC_SARA_H_ */
