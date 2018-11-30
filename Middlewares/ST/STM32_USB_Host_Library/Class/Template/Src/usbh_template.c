/**
  ******************************************************************************
  * @file    usbh_mtp.c
  * @author  MCD Application Team
  * @brief   This file is the MTP Layer Handlers for USB Host MTP class.
  *
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbh_template.h"
#include "cmsis_os.h"

/** @addtogroup USBH_LIB
* @{
*/

/** @addtogroup USBH_CLASS
* @{
*/

/** @addtogroup USBH_TEMPLATE_CLASS
* @{
*/

/** @defgroup USBH_TEMPLATE_CORE
* @brief    This file includes TEMPLATE Layer Handlers for USB Host TEMPLATE class.
* @{
*/

/** @defgroup USBH_TEMPLATE_CORE_Private_TypesDefinitions
* @{
*/
/**
* @}
*/


/** @defgroup USBH_TEMPLATE_CORE_Private_Defines
* @{
*/
/**
* @}
*/


/** @defgroup USBH_TEMPLATE_CORE_Private_Macros
* @{
*/
/**
* @}
*/


/** @defgroup USBH_TEMPLATE_CORE_Private_Variables
* @{
*/
/**
* @}
*/


/** @defgroup USBH_TEMPLATE_CORE_Private_FunctionPrototypes
* @{
*/

static USBH_StatusTypeDef USBH_TEMPLATE_InterfaceInit  (USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_TEMPLATE_InterfaceDeInit  (USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_TEMPLATE_Process(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_TEMPLATE_ClassRequest (USBH_HandleTypeDef *phost);

#define USB_TEMPLATE_CLASS	0x00U

USBH_ClassTypeDef  TEMPLATE_Class =
{
  "TEMPLATE",
  USB_TEMPLATE_CLASS,
  USBH_TEMPLATE_InterfaceInit,
  USBH_TEMPLATE_InterfaceDeInit,
  USBH_TEMPLATE_ClassRequest,
  USBH_TEMPLATE_Process
};
/**
* @}
*/


/** @defgroup USBH_TEMPLATE_CORE_Private_Functions
* @{
*/

/**
  * @brief  USBH_TEMPLATE_InterfaceInit
  *         The function init the TEMPLATE class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_TEMPLATE_InterfaceInit (USBH_HandleTypeDef *phost)
{

  return USBH_OK;
}



/**
  * @brief  USBH_TEMPLATE_InterfaceDeInit
  *         The function DeInit the Pipes used for the TEMPLATE class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_TEMPLATE_InterfaceDeInit (USBH_HandleTypeDef *phost)
{

  return USBH_OK;
}

/**
  * @brief  USBH_TEMPLATE_ClassRequest
  *         The function is responsible for handling Standard requests
  *         for TEMPLATE class.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_TEMPLATE_ClassRequest (USBH_HandleTypeDef *phost)
{

  return USBH_OK;
}


/**
  * @brief  USBH_TEMPLATE_Process
  *         The function is for managing state machine for TEMPLATE data transfers
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_TEMPLATE_Process (USBH_HandleTypeDef *phost)
{

  return USBH_OK;
}


/**
  * @brief  USBH_TEMPLATE_Init
  *         The function Initialize the TEMPLATE function
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_TEMPLATE_Init (USBH_HandleTypeDef *phost)
{
  USBH_StatusTypeDef Status = USBH_BUSY;
#if (USBH_USE_OS == 1U)
  osEvent event;

  event = osMessageGet( phost->os_event, osWaitForever );

  if( event.status == osEventMessage )
  {
    if(event.value.v == USBH_CLASS_EVENT)
    {
#else

  while ((Status == USBH_BUSY) || (Status == USBH_FAIL))
  {
    /* Host background process */
    USBH_Process(phost);
    if(phost->gState == HOST_CLASS)
    {
#endif
      Status = USBH_OK;
    }
  }
  return Status;
}

/**
  * @brief  USBH_TEMPLATE_IOProcess
  *         TEMPLATE TEMPLATE process
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_TEMPLATE_IOProcess (USBH_HandleTypeDef *phost)
{
  if (phost->device.is_connected == 1U)
  {
    if(phost->gState == HOST_CLASS)
    {
      USBH_TEMPLATE_Process(phost);
    }
  }

  return USBH_OK;
}

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


/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
