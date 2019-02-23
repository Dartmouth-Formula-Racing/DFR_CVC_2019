/*
 * cvc_sd.h
 *
 *  Created on: Dec 9, 2018
 *      Author: f002bc7
 */

#ifndef INC_CVC_SD_H_
#define INC_CVC_SD_H_

/* Includes ------------------------------------------------------------------*/

/* Standard includes. */
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

/* FreeRTOS Includes */
#include "FreeRTOS.h"

/* ST HAL includes. */
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_sd.h"


/* Defines ------------------------------------------------------------------*/

/* Configuration Defines */
#define BUS_4BITS							1

/* Pin Definitions */
#define SD_DETECT_PIN						GPIO_PIN_0
#define SD_DETECT_GPIO_PORT					GPIOG

/* DMA constants. */
#define SD_DMAx_Tx_CHANNEL					DMA_CHANNEL_4
#define SD_DMAx_Rx_CHANNEL					DMA_CHANNEL_4
#define SD_DMAx_Tx_STREAM					DMA2_Stream6
#define SD_DMAx_Rx_STREAM					DMA2_Stream3
#define SD_DMAx_Tx_IRQn						DMA2_Stream6_IRQn
#define SD_DMAx_Rx_IRQn						DMA2_Stream3_IRQn
#define __DMAx_TxRx_CLK_ENABLE				__DMA2_CLK_ENABLE
#define configSDIO_DMA_INTERRUPT_PRIORITY	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY )



/* The Instance of the MMC peripheral. */
#define	SDIO	SDMMC1

#define SDIO_CLOCK_EDGE_RISING				SDMMC_CLOCK_EDGE_RISING
#define SDIO_CLOCK_BYPASS_DISABLE			SDMMC_CLOCK_BYPASS_DISABLE
#define SDIO_CLOCK_POWER_SAVE_DISABLE		SDMMC_CLOCK_POWER_SAVE_DISABLE
#define SDIO_BUS_WIDE_1B					SDMMC_BUS_WIDE_1B
#define SDIO_BUS_WIDE_4B					SDMMC_BUS_WIDE_4B
#define SDIO_HARDWARE_FLOW_CONTROL_DISABLE	SDMMC_HARDWARE_FLOW_CONTROL_DISABLE

#define SD_SDIO_DISABLED					SD_SDMMC_DISABLED
#define SD_SDIO_FUNCTION_BUSY				SD_SDMMC_FUNCTION_BUSY
#define SD_SDIO_FUNCTION_FAILED				SD_SDMMC_FUNCTION_FAILED
#define SD_SDIO_UNKNOWN_FUNCTION			SD_SDMMC_UNKNOWN_FUNCTION

#define SDIO_IRQn							SDMMC1_IRQn


/* Type Defines ------------------------------------------------------------------*/

/**
  * @brief  SD status structure definition
  */
typedef enum
{
      BSP_SD_OK = 0x00,
      MSD_OK = 0x00,
      BSP_SD_ERROR = 0x01,
      BSP_SD_TIMEOUT
} BSP_SD_status_t;


/**
  * @brief SD Card information structure
  */
#define BSP_SD_CardInfo HAL_SD_CardInfoTypeDef


/* Function Prototypes ------------------------------------------------------------------*/

uint8_t BSP_SD_Init(void);
uint8_t BSP_SD_ReadBlocks_DMA(uint8_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks);
uint8_t BSP_SD_WriteBlocks_DMA(uint8_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks);
uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr);
uint8_t BSP_SD_GetCardState(void);
uint8_t BSP_SD_GetTransferState(void);
uint8_t BSP_SD_GetCardInfo(BSP_SD_CardInfo *pCardInfo);



#endif /* INC_CVC_SD_H_ */
