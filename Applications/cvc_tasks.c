/*
 * cvc_tasks.c
 *
 *  Created on: Dec 1, 2018
 *      Author: Leina McDermott
 */

/* Includes ------------------------------------------------------------------*/
#include "cvc_tasks.h"

/* Defines -------------------------------------------------------------------*/
#define TASKLIST_SIZE 	1

/* Private Variables ---------------------------------------------------------*/

/* list of all program tasks */
static task_t taskList[] = {
//		{demoTask, "demo", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL},		/* demo blinky task */
		{fatTask, "fatTest", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL},
};

/* Public Functions ----------------------------------------------------------*/

/**
 * Create all tasks in task list
 */
BaseType_t taskCreateAll()	{
	BaseType_t status = pdPASS;
	for (int i=0; i<TASKLIST_SIZE; i++)	{
		status = xTaskCreate(taskList[i].function,
							 taskList[i].name,
							 taskList[i].stackSize,
							 taskList[i].parameters,
							 taskList[i].priority,
							 taskList[i].handle);
		if (status != pdPASS)	{
			return status;
		}
	}
	return status;
}






