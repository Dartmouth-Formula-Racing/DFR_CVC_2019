/*
 * sd_diskio_dma_rtos.h
 *
 *  Created on: Feb 4, 2019
 *      Author: f002bc7
 */

/*
 * stm32f7_diskio_dma_rtos.h
 *
 *  Created on: Jan 9, 2019
 *      Author: schaefer
 */

#ifndef FATFS_DRIVERS_STM32F7_DISKIO_DMA_RTOS_H_
#define FATFS_DRIVERS_STM32F7_DISKIO_DMA_RTOS_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "ff_gen_drv.h"

 /* Imported constants */
 extern const Diskio_drvTypeDef  SD_Driver;

///**
//  * @brief SD Card information structure
//  */
//#define BSP_SD_CardInfo HAL_SD_CardInfoTypeDef
///**
//  * @}
//  */
//
///**
//  * @brief  SD status structure definition
//  */
//#define   MSD_OK                        ((uint8_t)0x00)
//#define   MSD_ERROR                     ((uint8_t)0x01)
//#define   MSD_ERROR_SD_NOT_PRESENT      ((uint8_t)0x02)
//
///**
//  * @brief  SD transfer state definition
//  */
//#define   SD_TRANSFER_OK                ((uint8_t)0x00)
//#define   SD_TRANSFER_BUSY              ((uint8_t)0x01)
//
///** @defgroup STM32F769I_EVAL_SD_Exported_Constants SD Exported Constants
//  * @{
//  */
//#define SD_PRESENT               ((uint8_t)0x01)
//#define SD_NOT_PRESENT           ((uint8_t)0x00)
//
//#define SD_CARD1                 ((uint32_t)0x00)
//#define SD_CARD2                 ((uint32_t)0x01)
//
//#define SD_DATATIMEOUT           ((uint32_t)100000000)
//
//#define SD_DetectIRQHandler()             HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8)
///**
//  * @}
//  */
//
///** @defgroup STM32F769I_EVAL_SD_Exported_Macro SD Exported Macro
//  * @{
//  */
///**
//  * @}
//  */
//
///** @defgroup STM32F769I_EVAL_SD_Exported_Functions SD Exported Functions
//  * @{
//  */
//uint8_t BSP_SD_Init(void);
//uint8_t BSP_SD_InitEx(uint32_t SdCard);
//uint8_t BSP_SD_DeInit(void);
//uint8_t BSP_SD_DeInitEx(uint32_t SdCard);
//uint8_t BSP_SD_ITConfig(void);
//uint8_t BSP_SD_ITConfigEx(uint32_t SdCard);
//uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout);
//uint8_t BSP_SD_ReadBlocksEx(uint32_t SdCard, uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout);
//uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks,  uint32_t Timeout);
//uint8_t BSP_SD_WriteBlocksEx(uint32_t SdCard, uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks,  uint32_t Timeout);
//uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks);
//uint8_t BSP_SD_ReadBlocks_DMAEx(uint32_t SdCard, uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks);
//uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks);
//uint8_t BSP_SD_WriteBlocks_DMAEx(uint32_t SdCard, uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks);
//uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr);
//uint8_t BSP_SD_EraseEx(uint32_t SdCard, uint32_t StartAddr, uint32_t EndAddr);
//uint8_t BSP_SD_GetCardState(void);
//uint8_t BSP_SD_GetCardStateEx(uint32_t SdCard);
//void    BSP_SD_GetCardInfo(BSP_SD_CardInfo *CardInfo);
//void    BSP_SD_GetCardInfoEx(uint32_t SdCard, BSP_SD_CardInfo *CardInfo);
//uint8_t BSP_SD_IsDetected(void);
//uint8_t BSP_SD_IsDetectedEx(uint32_t SdCard);
//
///* These functions can be modified in case the current settings (e.g. DMA stream)
//   need to be changed for specific application needs */
//void    BSP_SD_MspInit(SD_HandleTypeDef *hsd, void *Params);
//void    BSP_SD_MspDeInit(SD_HandleTypeDef *hsd, void *Params);
//void    BSP_SD_AbortCallback(uint32_t SdCard);
//void    BSP_SD_WriteCpltCallback(uint32_t SdCard);
//void    BSP_SD_ReadCpltCallback(uint32_t SdCard);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* FATFS_DRIVERS_STM32F7_DISKIO_DMA_RTOS_H_ */
