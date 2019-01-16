/*
 * cvc_tasks.c
 *
 *  Created on: Dec 1, 2018
 *      Author: Leina McDermott
 */

/* Includes ------------------------------------------------------------------*/
#include "cvc_tasks.h"
#include "demo.h"
#include "cvc_can.h"
#include "synchronous.h"

/* Defines -------------------------------------------------------------------*/
#define TASKLIST_SIZE 	5

/* Private Variables ---------------------------------------------------------*/

/* list of all program tasks */
static task_t taskList[] = {
		{demoTask, "demo", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL},		/* demo blinky task */
		//{CAN_Demo_Task,"CAN_demo", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL},	/* CAN demo task */
		{CAN_Rx_Task,"canRx", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL},		/* CAN Rx task */
		{CAN_Tx_Task,"canTx", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL},		/* CAN Tx task */
		{_10_ms_Task,"10ms", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL},
		{_50_ms_Task,"50ms", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL},
};

/* Public Functions ----------------------------------------------------------*/

/**
 * Create all tasks in task list
 */
BaseType_t taskCreateAll()	{
	BaseType_t status = pdPASS;

	configASSERT(TASKLIST_SIZE == sizeof(taskList)/sizeof(task_t));

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









