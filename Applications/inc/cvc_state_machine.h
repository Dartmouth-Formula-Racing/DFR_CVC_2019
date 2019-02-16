/*
 * cvc_state_machine.h
 *
 *  Created on: Feb 12, 2019
 *      Author: f002bc7
 */

#ifndef INC_CVC_STATE_MACHINE_H_
#define INC_CVC_STATE_MACHINE_H_


/* Defines ------------------------------------------------------------------------*/
#define BUZZER_TIMER_LOAD 60 // 2000/50 = 60 --> 2 seconds
#define PRE_CHARGE_TIMER_LOAD 50 // 2500/50 = 50 --> 2.5 seconds

/* Type Defines ------------------------------------------------------------------------*/
typedef enum cvc_state_e
{

	BAMO_INIT = 0,
	VOLTAGE_CHECK = 1,
	PRECHARGE = 2,
	READY_TO_DRIVE = 3,
	DRIVE = 4,
	FAULT = 5,

} cvc_state_t;

typedef enum cvc_fault_status_e
{

	CVC_OK = 0,
	CVC_WARNING = 1,
	CVC_RST_FAULT = 2,
	CVC_HARD_FAULT = 3,

} cvc_fault_status_t;

/* currently for debugging only */
typedef enum cvc_error_code_e
{
	NONE = 0,
	QUEUE_ERR = 1,
	CAN_ERR = 2,
	VOLTAGE_ERR = 3,
} cvc_error_code_t;

/* Variables to Export ------------------------------------------------------------------------*/
extern volatile cvc_state_t cvc_state;

/* Function Prototypes ------------------------------------------------------------------------*/
void state_machine();
void error_handler(cvc_fault_status_t fault, cvc_error_code_t error);

#endif /* INC_CVC_STATE_MACHINE_H_ */
