/*
 * cvc_state_machine.c
 *
 *  Created on: Feb 12, 2019
 *      Author: f002bc7
 */

/* Includes ------------------------------------------------------------------------*/

#include "cvc_state_machine.h"


/* Private Variables ------------------------------------------------------------------------*/

volatile cvc_state_t cvc_state = BAMO_INIT;
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
//	// add any checks that must be done in all states here
//	// - Dash BRB
//
//	if (cvc_fault != CVC_OK)
//	{
//		cvc_state = FAULT;
//	}
//
//	xSemaphoreTake(SPI_Inputs_Vector_Mutex, portMAX_DELAY);
//	Dash_BRB_Pressed = SPI_inputs_vector.Dash_BRB_press;
//	log_disable_temp = SPI_inputs_vector.Motor_enable;
//	TSMS_closed = SPI_inputs_vector.TSMS_closed;
//	xSemaphoreGive(SPI_Inputs_Vector_Mutex);
//
//	/* TODO: better fix for closing out file compare log_disable switch value to previous */
//	if (log_disable_prev != 0xFF && log_disable_temp != log_disable_prev)
//	{
//		log_disable_counter++;	/* debounce */
//	}
//	else
//	{
//		log_disable_prev = log_disable_temp;
//	}
//
//	if (log_disable_counter == 5)
//	{
//		LOG_FLAG = 0;
//	}
//
//	switch(cvc_state)
//	{
//	case BAMO_INIT:
//
//		/* set SPI outputs */
//		xSemaphoreTake(SPI_Outputs_Vector_Mutex, portMAX_DELAY);
//
//		SPI_outputs_vector.safety = 0;
//		SPI_outputs_vector.ready_to_drive = 0;
//		SPI_outputs_vector.rfg = 0;
//
//		xSemaphoreGive(SPI_Outputs_Vector_Mutex);
//
//		/* send Bamocar reset message */
//		bamo_var1_reset();
//
//		/* send Bamocar init messages */
//		if (bamo_init() == 1)
//		{
//			cvc_state = VOLTAGE_CHECK;
//		}
//		else
//		{
//			cvc_state = BAMO_INIT;
//		}
//
//		break;
//
//	case VOLTAGE_CHECK:
//
//		/* get batt & bamo voltages */
//		xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);
//
//		pack_voltage = ((float) CAN_inputs[BATT_VOLTAGE])/100.0f;
//		bus_voltage = ((float) CAN_inputs[BAMO_BUS_VOLTAGE])/50.4f;
//
//		xSemaphoreGive(CAN_Inputs_Vector_Mutex);
//
//		/* check batt & bamo voltages */
//		if (pack_voltage > MIN_BATTERY_VOLTAGE_THRESHOLD && bus_voltage < MAX_BAMO_BUS_VOLTAGE_THRESHOLD)
//		{
//			/* set SPI outputs */
//			xSemaphoreTake(SPI_Outputs_Vector_Mutex, portMAX_DELAY);
//
//			/* set safety-out pin */
//			SPI_outputs_vector.safety = 1;
//			SPI_outputs_vector.ready_to_drive = 0;
//			SPI_outputs_vector.rfg = 0;
//
//			xSemaphoreGive(SPI_Outputs_Vector_Mutex);
//
//			cvc_state = PRECHARGE;
//			voltage_check_timer = 0;
//			voltage_check_timer_started = 0;
//		}
//		else if ( voltage_check_timer_started == 0)
//		{
//			voltage_check_timer_started = 1;
//			voltage_check_timer = VOLTAGE_CHECK_TIMEOUT_LOAD;
//			cvc_state = VOLTAGE_CHECK;
//		}
//		else if ( voltage_check_timer_started == 1 && voltage_check_timer == 0)
//		{
//			cvc_error_handler(CVC_HARD_FAULT, VOLTAGE_ERR);
//			voltage_check_timer = 0;
//			voltage_check_timer_started = 0;
//		}
//		else
//		{
//			cvc_state = VOLTAGE_CHECK;
//			voltage_check_timer--;
//		}
//
//		break;
//
//	case PRECHARGE:
//
//		/* set SPI outputs */
//		xSemaphoreTake(SPI_Outputs_Vector_Mutex, portMAX_DELAY);
//
//		SPI_outputs_vector.safety = 1;
//		SPI_outputs_vector.ready_to_drive = 0;
//		SPI_outputs_vector.rfg = 0;
//
//		xSemaphoreGive(SPI_Outputs_Vector_Mutex);
//
//		/* get batt & bamo voltages */
//		xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);
//
//		pack_voltage = (float) CAN_inputs[BATT_VOLTAGE]/100.0f;
//		bus_voltage = (float) CAN_inputs[BAMO_BUS_VOLTAGE]/50.4;	//*96.0f/3600.0f;
//
//		xSemaphoreGive(CAN_Inputs_Vector_Mutex);
//
//		/* wait for 90% precharge */
//		if (bus_voltage >= pack_voltage * 0.9f)
//		{
//			precharge_90p_voltage = 1;
//		}
//
//		/* start precharge timer and wait */
//		if (precharge_90p_voltage && !precharge_timer_started)
//		{
//			precharge_timer = PRE_CHARGE_TIMER_LOAD;
//			precharge_timer_started = 1;
//		}
//		else if (precharge_90p_voltage && precharge_timer_started)
//		{
//			precharge_timer--;
//		}
//
//		if (precharge_timer_started && precharge_timer == 0)
//		{
//			precharge_complete = 1;
//		}
//
//		/* send Bamocar set message when precharge complete to close second AIR */
//		if (precharge_complete)
//		{
//			bamo_var1_set();
//			cvc_state = READY_TO_DRIVE;
//			precharge_90p_voltage = 0;
//			precharge_timer_started = 0;
//			precharge_complete = 0;
//		}
//		else
//		{
//			cvc_state = PRECHARGE;
//		}
//
//		/* TODO: alert driver AIRs are closed (MOTEC alert) */
//
//		break;
//
//	case READY_TO_DRIVE:
//
//		/* get push button state */
//		xSemaphoreTake(SPI_Inputs_Vector_Mutex, portMAX_DELAY);
//
//		push_button = SPI_inputs_vector.Ready_to_drive;
//
//		xSemaphoreGive(SPI_Inputs_Vector_Mutex);
//
//		/* wait for push button */
//		if (push_button != 0 && !buzzer_timer_started)
//		{
//			buzzer_timer = BUZZER_TIMER_LOAD;
//			buzzer_timer_started = 1;
//		}
//		else if (buzzer_timer_started)
//		{
//			/* buzzer sound ON */
//			ready_to_drive = 1;
//
//			/* decrement counter */
//			buzzer_timer--;
//		}
//
//		/* check buzzer timer */
//		if (buzzer_timer_started && buzzer_timer == 0)
//		{
//			ready_to_drive = 0;
//			buzzer_timer_started = 0;
//			cvc_state = DRIVE;
//		}
//		else
//		{
//			cvc_state = READY_TO_DRIVE;
//		}
//
//		/* set SPI outputs */
//		xSemaphoreTake(SPI_Outputs_Vector_Mutex, portMAX_DELAY);
//
//		/* set safety-out pin */
//		SPI_outputs_vector.safety = 1;
//		SPI_outputs_vector.ready_to_drive = ready_to_drive;
//		SPI_outputs_vector.rfg = 0;
//
//		xSemaphoreGive(SPI_Outputs_Vector_Mutex);
//
//		/* check for dash BRB */
//		if (Dash_BRB_Pressed == 1)
//		{
//			cvc_state = DASH_BRB;
//		}
//
//
//		/* check for open Tractive System Master Switch */
//		if (TSMS_closed == 0)
//		{
//			cvc_state = PRECHARGE;
//		}
//
//		break;
//
//	case DRIVE:
//
//		/* set SPI outputs */
//		xSemaphoreTake(SPI_Outputs_Vector_Mutex, portMAX_DELAY);
//
//		/* set safety-out pin */
//		SPI_outputs_vector.safety = 1;
//		SPI_outputs_vector.ready_to_drive = 0;
//		SPI_outputs_vector.rfg = 1;
//
//		xSemaphoreGive(SPI_Outputs_Vector_Mutex);
//
//		cvc_state = DRIVE;
//
//		/* check for dash BRB */
//		if (Dash_BRB_Pressed == 1)
//		{
//			cvc_state = DASH_BRB;
//		}
//
//		/* check for open Tractive System Master Switch */
//		if (TSMS_closed == 0)
//		{
//			cvc_state = PRECHARGE;
//		}
//
//		break;
//
//	case FAULT:
//
//		/* set SPI outputs */
//		xSemaphoreTake(SPI_Outputs_Vector_Mutex, portMAX_DELAY);
//
//		/* set safety-out pin */
//		SPI_outputs_vector.safety = 0;
//		SPI_outputs_vector.ready_to_drive = 0;
//		SPI_outputs_vector.rfg = 0;
//
//		xSemaphoreGive(SPI_Outputs_Vector_Mutex);
//
//		break;
//
//	case DASH_BRB:
//
//		/* set SPI outputs */
//		xSemaphoreTake(SPI_Outputs_Vector_Mutex, portMAX_DELAY);
//
//		/* set safety-out pin */
//		SPI_outputs_vector.safety = 1;
//		SPI_outputs_vector.ready_to_drive = 0;
//		SPI_outputs_vector.rfg = 0;
//
//		xSemaphoreGive(SPI_Outputs_Vector_Mutex);
//
//		precharge_90p_voltage = 0;
//		precharge_timer_started = 0;
//		precharge_complete = 0;
//
//		if (Dash_BRB_Pressed == 0)
//		{
//			cvc_state = PRECHARGE;
//
//			/* send Bamocar reset message */
//			bamo_var1_reset();
//		}
//
//		break;
//
//	default:
//		break;
//	}
}


void safety_monitor(void)
{
//	monitor_engine();
//	monitor_tps();
//	//monitor_bamocar()
//	//monitor_bms()
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
