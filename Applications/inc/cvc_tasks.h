/*
 * cvc_tasks.h
 *
 *  Created on: Dec 1, 2018
 *      Author: f002bc7
 */

#ifndef CVC_TASKS_H_
#define CVC_TASKS_H_

#include "freeRTOS.h"
#include "task.h"
#include "demo.h"
#include "cvc_logging.h"


/*********************************************************************************
 	 	 	 	 	 	 	 	   Type Defines
**********************************************************************************/

typedef struct	{
	TaskFunction_t function;		/* The function that implements the task. */
	const char * const name;		/* The text name assigned to the task - for debug only as it is not used by the kernel. */
	const uint16_t stackSize;		/* The size of the stack to allocate to the task. */
	void * const parameters;		/* The parameter passed to the task - not used in this case. */
	UBaseType_t priority;			/* The priority assigned to the task. */
	TaskHandle_t * const handle;	/* Reference to created task */
} task_t;


/*********************************************************************************
 	 	 	 	 	 	 	  Function Declarations
**********************************************************************************/
BaseType_t taskCreateAll();


#endif /* CVC_TASKS_H_ */


