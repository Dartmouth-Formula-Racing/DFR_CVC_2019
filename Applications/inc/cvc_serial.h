/*
 * cvc_serial.h
 *
 *  Created on: Dec 5, 2018
 *      Author: f002bc7
 */

#ifndef INC_CVC_SERIAL_H_
#define INC_CVC_SERIAL_H_


/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_system.h"
#include "stm32f7xx_ll_utils.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_exti.h"
#include "stm32f7xx_ll_usart.h"
#include "stm32f7xx_ll_pwr.h"
#include "stm32f7xx_hal.h"
#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** USE_PRINTF Determines whether printf (blocking) or console_write (non-blocking) is used to communicate with console
 *
 * USE_PRINTF == 1: The USART is configured as follows:
    - BaudRate = 9600 baud
    - Word Length = 8 Bits (7 data bit + 1 parity bit)
    - One Stop Bit
    - Odd parity
    - Hardware flow control disabled (RTS and CTS signals)
 *
 * USE_PRINTF == 0: The USART is configured as follows:
 	- BaudRate = 115200 bauds
 	- Word Length = 8 bits data
 	- One stop bit
 	- No parity
 	- Hardware flow control disabled (RTS and CTS signals)
 */

#define USE_PRINTF       1

/* Private definitions covering GPIO clock and USART pins
   depending on selected USART instance. */

/* USART3 instance is used. (TX on PD.08, RX on PD.09)
   (please ensure that USART communication between the target MCU and ST-LINK MCU is properly enabled
    on HW board in order to support Virtual Com Port) */

#if USE_PRINTF == 0

#define USARTx_INSTANCE               USART3
#define USARTx_CLK_ENABLE()           LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3)
#define USARTx_CLK_SOURCE()           LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1)
#define USARTx_IRQn                   USART3_IRQn
#define USARTx_IRQHandler             USART3_IRQHandler

#define USARTx_GPIO_CLK_ENABLE()      LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD)   /* Enable the peripheral clock of GPIOD */
#define USARTx_TX_PIN                 LL_GPIO_PIN_8
#define USARTx_TX_GPIO_PORT           GPIOD
#define USARTx_SET_TX_GPIO_AF()       LL_GPIO_SetAFPin_8_15(GPIOD, LL_GPIO_PIN_8, LL_GPIO_AF_7)
#define USARTx_RX_PIN                 LL_GPIO_PIN_9
#define USARTx_RX_GPIO_PORT           GPIOD
#define USARTx_SET_RX_GPIO_AF()       LL_GPIO_SetAFPin_8_15(GPIOD, LL_GPIO_PIN_9, LL_GPIO_AF_7)
#define APB_Div 4

#else

/* Definition for USARTx clock resources */
#define USARTx                           USART3
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_8
#define USARTx_TX_GPIO_PORT              GPIOD
#define USARTx_TX_AF                     GPIO_AF7_USART3
#define USARTx_RX_PIN                    GPIO_PIN_9
#define USARTx_RX_GPIO_PORT              GPIOD
#define USARTx_RX_AF                     GPIO_AF7_USART3

#endif /* USE_PRINTF == 0 */
#define MAX_STRING_LENGTH		100U

/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
/* IRQ Handler treatment functions */
void USART_TXEmpty_Callback(void);
void USART_CharTransmitComplete_Callback(void);
void Error_Callback(void);
void console_write(char * message);
void Configure_USART(void);


#endif /* INC_CVC_SERIAL_H_ */
