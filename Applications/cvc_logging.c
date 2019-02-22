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
static FIL LogFile; /* File object */
static FIL CopyFile;
static char SDPath[4]; /* SD disk logical drive path */
static uint32_t wbytes; /* File write counts */
static uint32_t rbytes; /* File read counts */

uint32_t logging_outputs[10] = {11111,22222,33333,44444,55555,66666,77777,88888,99999,00000};

//#define WRITER

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
	BSP_LED_Init(LED_RED);
}

void loggingTask(void * parameters)
{
	char *pbuff = (char *) buff;
	uint8_t n_outputs = sizeof(logging_outputs)/sizeof(uint32_t);
	uint8_t n_writes = 0;

	while(1)
	{

#ifdef WRITER
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
		if (f_open(&, "STM32.csv", FA_OPEN_APPEND | FA_WRITE) == FR_OK)
		{
			if(f_write(&LogFile, pbuff, i, (void *)&wbytes) != FR_OK)
			{
				f_close(&LogFile);
				logging_error();
			}
			f_close(&LogFile);

			/* increase number of writes */
			n_writes++;

			BSP_LED_On(LED_GREEN);
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
			BSP_LED_On(LED_RED);
			vTaskSuspend(NULL);
		}

#else
		/* read back data to check for success */
		if (f_open(&LogFile, "STM32.csv", FA_OPEN_EXISTING | FA_READ) == FR_OK && f_open(&CopyFile, "STM32_copy.csv", FA_OPEN_ALWAYS | FA_WRITE) == FR_OK)
		{
			FSIZE_t file_size = f_size(&LogFile);
			int buff_size = sizeof(buff);

			int n = file_size / buff_size;
			int r = file_size % buff_size;

			for (int j = 0; j <= n; j++)
			{

				if(f_read(&LogFile, pbuff, (j < n) ? buff_size : r, (void *)&rbytes) != FR_OK)
				{
					f_close(&LogFile);
					f_close(&CopyFile);
					logging_error();
				}
				if(f_write(&CopyFile, pbuff, rbytes, (void *)&wbytes) != FR_OK)
				{
					f_close(&LogFile);
					f_close(&CopyFile);
					logging_error();
				}
			}

			f_close(&LogFile);
			f_close(&CopyFile);

			BSP_LED_On(LED_BLUE);
		}
		else
		{
			logging_error();
		}

		vTaskSuspend(NULL);

#endif /* WRITER */

	}

}

void logging_error()
{
	while(1);
}

