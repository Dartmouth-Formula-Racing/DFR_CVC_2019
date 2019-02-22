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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void logging_error();

//static uint8_t wtext[] = "Leina,Alex,Trammell,John\n1,2,3,4\n"; /* File write buffer */
static uint32_t buff[100U];
static FATFS SD_FatFs; /* File system object for User logical drive */
static FIL MyFile; /* File object */
static char SDPath[4]; /* SD disk logical drive path */
static uint32_t wbytes; /* File write counts */
static uint32_t rbytes; /* File read counts */

uint32_t logging_outputs[10] = {11111,22222,33333,44444,55555,66666,77777,88888,99999,00000};


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

void loggingTask(void * parameters)
{
	char *pbuff = (char *) buff;
	uint8_t n_outputs = sizeof(logging_outputs)/sizeof(uint32_t);
	uint8_t n_writes = 0;

	while(1)
	{

		uint8_t i = 0;

		TickType_t ticks = xTaskGetTickCount();

		/* add header*/
		if (n_writes == 0)
		{
			strcpy(pbuff+i, "ticks,one,two,three,four,five,six,seven,eight,nine,zero\n");
			i+=strlen(pbuff+i);
		}

		/* add current ticks to log file */
		utoa(ticks, pbuff+i, 10);
		i+=strlen(pbuff+i);
		*(pbuff+i) = 44;	// insert ASCII code for "," character
		i++;

		/* loop through logging outputs and write to file */
		// TODO: add critical sections around fopen->fclose
		for (uint8_t j = 0; j < n_outputs; j++)
		{
			utoa(logging_outputs[j], pbuff+i, 10);
			i+=strlen(pbuff+i);

			if (j<n_outputs-1)
			{
				*(pbuff+i) = 44;	// insert ASCII code for "," character
			}
			else
			{
				*(pbuff+i) = 10;		// inset ASCII code for null terminator
			}
			i++;
		}

		/* write data to file */
		if (f_open(&MyFile, "STM32.csv", FA_OPEN_APPEND | FA_WRITE) == FR_OK)
		{
			if(f_write(&MyFile, pbuff, i, (void *)&wbytes) != FR_OK)
			{
				f_close(&MyFile);
				logging_error();
			}
			f_close(&MyFile);

			/* increase number of writes */
			n_writes++;

			BSP_LED_On(LED_GREEN);
		}
		else
		{
			logging_error();
		}

		/* read back data to check for success */
		if (f_open(&MyFile, "STM32.csv", FA_OPEN_EXISTING | FA_READ) == FR_OK)
		{
			char *result = pvPortMalloc(wbytes);
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


		if (ticks < 10000)
		{
			vTaskDelay(1000);
		}
		else
		{
			BSP_LED_On(LED_BLUE);
			vTaskSuspend(NULL);
		}
	}
}

void logging_error()
{
	while(1);
}

