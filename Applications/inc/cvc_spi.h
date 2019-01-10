/*
 * cvc_spi.h
 *
 *  Created on: Jan 9, 2019
 *      Author: f002ccx
 */

#ifndef INC_CVC_SPI_H_
#define INC_CVC_SPI_H_

/* Temporary defines of LED blink times */
 /**
  * @brief Toggle periods for various blinking modes
  */
#define LED_BLINK_FAST  200
#define LED_BLINK_SLOW  500
#define LED_BLINK_ERROR 1000

/**
  * @brief LED1
  */
#define LED1_PIN                           LL_GPIO_PIN_0
#define LED1_GPIO_PORT                     GPIOB
#define LED1_GPIO_CLK_ENABLE()             LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB)



/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_system.h"
#include "stm32f7xx_ll_utils.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_exti.h"
#include "stm32f7xx_ll_spi.h"
#include "stm32f7xx_ll_pwr.h"

/* Function prototypes -----------------------------------------------*/
void Configure_SPI(void);
void Activate_SPI(void);

void SPI1_Rx_Callback(void);
void SPI1_Tx_Callback(void);
void SPI1_TransferError_Callback(void);

void WaitAndCheckEndOfTransfer(void);
uint8_t Buffercmp8(uint8_t* pBuffer1, uint8_t* pBuffer2, uint8_t BufferLength);


#endif /* INC_CVC_SPI_H_ */
