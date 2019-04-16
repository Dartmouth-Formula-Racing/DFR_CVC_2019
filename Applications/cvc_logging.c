/*
 * cvc_logging.c
 *
 *  Created on: Feb 6, 2019
 *      Author: f002bc7
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx_nucleo_144.h"
#include "FreeRTOS.h"
#include "task.h"
#include "ff_test.h"
#include "cvc_logging.h"
#include "usbh_core.h"
#include "usbh_msc.h"
#include "cvc_usbh.h"

extern

void fatTask(void * parameters)
{
	osEvent event;

	/* USB INITIALIZATIONS */
	/* Start Host Library */
	USBH_Init(&hUSBHost, USBH_UserProcess, 0);

	/* Add Supported Class */
	USBH_RegisterClass(&hUSBHost, USBH_MSC_CLASS);

	/* Start Host Process */
	USBH_Start(&hUSBHost);

	/* Create Application Queue */
	osMessageQDef(osqueue, 1, uint16_t);
	AppliEvent = osMessageCreate(osMessageQ(osqueue), NULL);

	while(1)
	{
		event = osMessageGet(AppliEvent, osWaitForever);

		if(event.status == osEventMessage)
		{
		  switch(event.value.v)
		  {
		  case APPLICATION_DISCONNECT:
			Appli_state = APPLICATION_DISCONNECT;
			break;

		  case APPLICATION_READY:
			Appli_state = APPLICATION_READY;

			/* test disk driver once ready */
			function_test_main();

			break;
		  default:
			break;
		  }
		}


		vTaskSuspend(NULL);
	}
}

