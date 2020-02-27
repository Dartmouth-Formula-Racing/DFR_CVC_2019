/*
 * cvc_state_machine.c
 *
 *  Created on: Feb 12, 2019
 *      Author: f002bc7
 */

/* Includes ------------------------------------------------------------------------*/

#include "cvc_state_machine.h"


/* Private Variables ------------------------------------------------------------------------*/

volatile cvc_state_t cvc_state = PRECHARGE;
static cvc_fault_status_t cvc_fault = CVC_OK;
static cvc_error_code_t cvc_error = NONE;

static int Dash_BRB_Pressed = 0;
static int TSMS_closed = 1;
static int voltage_check_timer = 0;
static uint8_t voltage_check_timer_started = 0;
static int precharge_timer = 0;
static uint8_t precharge_90p_voltage = 0;
static uint8_t precharge_timer_started = 0;
static uint8_t precharge_complete = 0;
static int buzzer_timer = 0;
static uint8_t buzzer_timer_started = 0;

static uint8_t push_button = 0;
static uint8_t ready_to_drive = 0;

static float pack_voltage;
static float bus_voltage;

static uint8_t log_disable_prev = 0xFF;
static uint8_t log_disable_temp = 0xFF;
static uint8_t log_disable_counter = 0;


void state_machine()
{

	// add any checks that must be done in all states here

	switch(cvc_state)
	{
	case PRECHARGE:



		/* get batt & bamo voltages */
		xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);

		pack_voltage = (float) 50.0;
		bus_voltage = (float) ((int)CAN_inputs[DC_BUS_VOLTAGE])/10.0;	//*96.0f/3600.0f;

		xSemaphoreGive(CAN_Inputs_Vector_Mutex);

		/* wait for 90% precharge */
		if (bus_voltage >= pack_voltage * 0.9f)
		{
			precharge_90p_voltage = 1;
		}

		/* start precharge timer and wait */
		if (precharge_90p_voltage && !precharge_timer_started)
		{
			precharge_timer = PRE_CHARGE_TIMER_LOAD;
			precharge_timer_started = 1;
		}
		else if (precharge_90p_voltage && precharge_timer_started)
		{
			precharge_timer--;
		}

		if (precharge_timer_started && precharge_timer == 0)
		{
			precharge_complete = 1;
		}

		/* send Bamocar set message when precharge complete to close second AIR */
		if (precharge_complete)
		{

			cvc_state = DRIVE;
			precharge_90p_voltage = 0;
			precharge_timer_started = 0;
			precharge_complete = 0;
		}
		else
		{
			cvc_state = PRECHARGE;
		}

		/* TODO: alert driver AIRs are closed (MOTEC alert) */

		break;


	case DRIVE:

		/* set SPI outputs */

		cvc_state = DRIVE;

		pack_voltage = (float) 50.0;
		bus_voltage = (float) ((int)CAN_inputs[DC_BUS_VOLTAGE])/10.0;

		/* check for open Tractive System Master Switch */
		if (bus_voltage <= 0.9 * pack_voltage)
		{
			cvc_state = PRECHARGE;
		}

		break;

	default:
		break;
	}

}


void safety_monitor(void)
{

	//monitor_bamocar()
	//monitor_bms()

}


void cvc_error_handler(cvc_fault_status_t fault, cvc_error_code_t error)
{
	// TODO: account for pre-scheduler errors (init fault handler, probably just infinite loop)

	BSP_LED_On(LED_RED);
	cvc_fault = fault;
	cvc_error = error;
}

void init_fault_handler(void)
{
	BSP_LED_On(LED_RED);
	while(1);
}
