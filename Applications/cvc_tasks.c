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
#include "cvc_spi.h"
#include "synchronous.h"
#include "cvc_state_machine.h"
#include "stm32f7xx_nucleo_144.h"

/* Defines -------------------------------------------------------------------*/
#define TASKLIST_SIZE 	1

/* Private Function Prototypes ---------------------------------------------------------*/
void Init_Task(void *parameters);

/* Private Variables ---------------------------------------------------------*/
static task_t init = {Init_Task, "init", 2*configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL};	/* task to create all other tasks */

/* list of all program tasks */
static task_t taskList[] = {
//		{demoTask, "demo", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL},		/* demo blinky task */
//		{CAN_Rx_Task,"canRx", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL},		/* CAN Rx task */
//		{CAN_Tx_Task,"canTx", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL},		/* CAN Tx task */
//		{_10_ms_Task,"10ms", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL},
//		{_20_ms_Task,"20ms", 2*configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL},
		{_50_ms_Task,"50ms", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL},
//		{PLC_Routine_Task, "plc_task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL },
};

/* Public Functions ----------------------------------------------------------*/
/**
 * Create initialization task
 */
void initTaskCreate(void)
{
	if (xTaskCreate(init.function, init.name, init.stackSize, init.parameters, init.priority, init.handle) != pdPASS)
	{
		init_fault_handler();
	}
}

/**
 * Call initialization functions and create application tasks
 */
void Init_Task(void *parameters)
{
	/* 1-second delay necessary to allow board to fully power up */
	vTaskDelay(1000);

	/* initialize data logging */
	if (logging_init() == pdPASS)
	{
		/* Initialize CAN */
		CAN_Init();

		/* Initialize SPI */
		Configure_SPI();
		Activate_SPI();

		/* Create functional tasks */
		if (taskCreateAll() != pdPASS)
		{
			init_fault_handler();
		}
	}
	else
	{
		init_fault_handler();
	}

	/* delete task */
	vTaskDelete(NULL);

	while(1)
	{
		vTaskSuspend(NULL);
	}
}

/**
 * @brief Create all tasks in task list
 */
BaseType_t taskCreateAll()
{
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

/**
 * @brief handle FreeRTOS task stack overflow exceptions
 */
void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    signed char *pcTaskName )
{
	BSP_LED_On(LED_RED);
	for(;;);
}







