/*
 * cvc_state_machine.h
 *
 *  Created on: Feb 12, 2019
 *      Author: f002bc7
 */

#ifndef INC_CVC_STATE_MACHINE_H_
#define INC_CVC_STATE_MACHINE_H_

/* Includes ------------------------------------------------------------------------*/

#include <stdint.h>
#include "cvc_spi.h"
#include "cvc_can.h"
#include "stm32f7xx_nucleo_144.h"
#include "cvc_logging.h"

/* Defines ------------------------------------------------------------------------*/
//#define BUZZER_TIMER_LOAD 40 // 2 seconds --> (2 s)/(0.05 s) = 40 ticks
//#define PRE_CHARGE_TIMER_LOAD 50 // 2.5 seconds --> (2.5 s)/(0.05 s) = 50 ticks

#define BUZZER_TIMER_LOAD 200 // 2 seconds --> (2 s)/(0.01 s) = 200 ticks
#define PRE_CHARGE_TIMER_LOAD 250 // 2.5 seconds --> (2.5 s)/(0.01 s) = 250 ticks

#define VOLTAGE_CHECK_TIMEOUT_LOAD 1000 // 10 seconds --> (10 s)/(0.01 s) = 1000 ticks
#define MIN_BATTERY_VOLTAGE_THRESHOLD	240 // 240 V minimum for battery to pre-charge
#define MAX_BAMO_BUS_VOLTAGE_THRESHOLD	20	// 20 V maximum for bamocar to pre-charge

/* Type Defines ------------------------------------------------------------------------*/
typedef enum cvc_state_e
{

	PRECHARGE = 0,
	DRIVE = 1,
	NEUTRAL = 2,
	REVERSE = 3,

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
	ENGINE_ERR = 4,
	LOGGING_ERR = 5,
	TPS_ERR = 6,
} cvc_error_code_t;

/* Variables to Export ------------------------------------------------------------------------*/
extern volatile cvc_state_t cvc_state;

/* Function Prototypes ------------------------------------------------------------------------*/
void state_machine();
void cvc_error_handler(cvc_fault_status_t fault, cvc_error_code_t error);
void safety_monitor(void);
void init_fault_handler(void);


#endif /* INC_CVC_STATE_MACHINE_H_ */
