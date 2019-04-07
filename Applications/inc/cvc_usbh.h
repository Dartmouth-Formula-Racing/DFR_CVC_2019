/*
 * cvc_usbh.h
 *
 *  Created on: Apr 6, 2019
 *      Author: f002bc7
 */

#ifndef INC_CVC_USBH_H_
#define INC_CVC_USBH_H_

#include "usbh_core.h"
#include "usbh_msc.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "usbh_diskio_dma.h"

extern FATFS USBH_fatfs;
extern USBH_HandleTypeDef hUSBHost;
extern osMessageQId AppliEvent;
extern MSC_ApplicationTypeDef Appli_state;

typedef enum {
  APPLICATION_IDLE,
  APPLICATION_DISCONNECT = 1,
  APPLICATION_READY,
}MSC_ApplicationTypeDef;


/* Function Prototypes */
void USBH_UserProcess(USBH_HandleTypeDef * phost, uint8_t id);


#endif /* INC_CVC_USBH_H_ */
