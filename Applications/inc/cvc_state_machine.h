/*
 * cvc_state_machine.h
 *
 *  Created on: Feb 12, 2019
 *      Author: f002bc7
 */

#ifndef INC_CVC_STATE_MACHINE_H_
#define INC_CVC_STATE_MACHINE_H_


typedef enum cvc_state_e
{
	BAMO_INIT = 0,
	PRECHARGE = 1,
	READY_TO_DRIVE = 2,
	DRIVE = 3,
	FAULT = 4,
} cvc_state_t;

extern volatile cvc_state_t cvc_state;


#endif /* INC_CVC_STATE_MACHINE_H_ */
