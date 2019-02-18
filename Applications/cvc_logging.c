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
#include "diskio.h"
#include "ff_gen_drv.h"
#include "sd_diskio_dma.h"

void logging_error();

static uint8_t wtext[] = "Leina,Alex,Trammell,John\n1,2,3,4\n"; /* File write buffer */
static FATFS SD_FatFs; /* File system object for User logical drive */
static FIL MyFile; /* File object */
static char SDPath[4]; /* SD disk logical drive path */
static uint32_t wbytes; /* File write counts */
static uint32_t rbytes; /* File read counts */


void logging_init()
{
	/* link/initialize SD driver */
	if (FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
	{
		/* mount disk */
		if (f_mount(&SD_FatFs, (char const*) SDPath, 0) != FR_OK)
		{
			logging_error();
		}
	}
	else
	{
		logging_error();
	}
	BSP_LED_Init(LED_GREEN);
	BSP_LED_Init(LED_BLUE);
}

void fatTask(void * parameters)
{
	while(1)
	{
		if (f_open(&MyFile, "STM32.csv", FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
		{
			if(f_write(&MyFile, wtext, sizeof(wtext), (void *)&wbytes) != FR_OK)
			{
				f_close(&MyFile);
				logging_error();
			}
			f_close(&MyFile);

			BSP_LED_On(LED_GREEN);
		}
		else
		{
			logging_error();
		}

//		FSIZE_t size = f_size(&MyFile);

		if (f_open(&MyFile, "STM32.csv", FA_OPEN_EXISTING | FA_READ) == FR_OK)
		{
			uint8_t *result = pvPortMalloc(wbytes);
			if(f_read(&MyFile, result, wbytes, (void *)&rbytes) != FR_OK)
			{
				f_close(&MyFile);
				vPortFree(result);
				logging_error();
			}
			f_close(&MyFile);

			BSP_LED_On(LED_BLUE);
			vPortFree(result);
		}
		else
		{
			logging_error();
		}


		vTaskSuspend(NULL);
	}
}

void logging_error()
{
	while(1);
}

