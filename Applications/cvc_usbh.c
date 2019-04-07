/*
 * cvc_usbh.c
 *
 *  Created on: Apr 6, 2019
 *      Author: f002bc7
 */

#include "cvc_usbh.h"


/* Private variables ---------------------------------------------------------*/
USBH_HandleTypeDef hUSBHost;
MSC_ApplicationTypeDef Appli_state = APPLICATION_IDLE;
char USBDISKPath[4];            /* USB Host logical drive path */
osMessageQId AppliEvent;
FATFS USBH_fatfs;


/**
  * @brief  User Process
  * @param  phost: Host Handle
  * @param  id: Host Library user message ID
  * @retval None
  */
void USBH_UserProcess(USBH_HandleTypeDef * phost, uint8_t id)
{
  switch (id)
  {
  case HOST_USER_SELECT_CONFIGURATION:
    break;

  case HOST_USER_DISCONNECTION:
    osMessagePut(AppliEvent, APPLICATION_DISCONNECT, 0);
    if (f_mount(NULL, "", 0) != FR_OK)
    {
    	while(1);
    }
    if (FATFS_UnLinkDriver(USBDISKPath) != 0)
    {
    	while(1);
    }
    break;

  case HOST_USER_CLASS_ACTIVE:
    osMessagePut(AppliEvent, APPLICATION_READY, 0);
    break;

  case HOST_USER_CONNECTION:
    if (FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == 0)
    {
      if (f_mount(&USBH_fatfs, "", 0) != FR_OK)
      {
    	  while(1);
      }
    }
	break;

  default:
    break;
  }
}
