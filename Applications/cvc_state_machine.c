/*
 * cvc_state_machine.c
 *
 *  Created on: Feb 12, 2019
 *      Author: f002bc7
 */

/* Includes ------------------------------------------------------------------------*/

#include "cvc_state_machine.h"
#include "cvc_can.h"


/* Private Variables ------------------------------------------------------------------------*/

//volatile cvc_state_t cvc_state = PRECHARGE;
volatile cvc_state_t cvc_state = DRIVE;
volatile cvc_state_t drive_state = NEUTRAL;
volatile cvc_fault_status_t cvc_fault = CVC_OK;
volatile cvc_error_code_t cvc_error = NONE;

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

	int16_t xBuffer = (CAN_inputs[X_AXIS_ACCELERATION]);
	int16_t yBuffer = (CAN_inputs[Y_AXIS_ACCELERATION]);
	int16_t zBuffer = (CAN_inputs[Z_AXIS_ACCELERATION]);

	float xAccel = ((float)xBuffer)*0.000244141;
	float yAccel = ((float)yBuffer)*0.000244141;
	float zAccel = ((float)zBuffer)*0.000244141;

	switch(cvc_state) {

	case PRECHARGE:



//		/* get batt & bamo voltages */
//		xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);
//
//		pack_voltage = (float) 50.0;
//		bus_voltage = (float) ((int)CAN_inputs[DC_BUS_VOLTAGE])/10.0;	//*96.0f/3600.0f;
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
//
//			cvc_state = DRIVE;
//			precharge_90p_voltage = 0;
//			precharge_timer_started = 0;
//			precharge_complete = 0;
//		}
//		else
//		{
//			cvc_state = PRECHARGE;
//		}
//
//		/* get push button state */
//		xSemaphoreTake(SPI_Inputs_Vector_Mutex, portMAX_DELAY);
//
//		push_button = SPI_inputs_vector.Ready_to_drive;
//		if (push_button) {
//			cvc_state = DRIVE;
//		}
//
//		xSemaphoreGive(SPI_Inputs_Vector_Mutex);

		/* TODO: alert driver AIRs are closed (MOTEC alert) */

		xSemaphoreTake(SPI_Outputs_Vector_Mutex, portMAX_DELAY);

		/* set safety-out pin */
		SPI_outputs_vector.cvc_err = 0;
		SPI_outputs_vector.cvc_warn = 0;

		xSemaphoreGive(SPI_Outputs_Vector_Mutex);

//		/* get push button state */
//		xSemaphoreTake(SPI_Inputs_Vector_Mutex, portMAX_DELAY);
//
//		push_button = SPI_inputs_vector.Ready_to_drive;
//		if (push_button) {
//			cvc_state = DRIVE;
//		}
//
//		xSemaphoreGive(SPI_Inputs_Vector_Mutex);

		xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);


		if (zAccel <= 0) {
			cvc_state = DRIVE;
		}

		xSemaphoreGive(CAN_Inputs_Vector_Mutex);

		break;


	case DRIVE:

		xSemaphoreTake(SPI_Outputs_Vector_Mutex, portMAX_DELAY);
		xSemaphoreGive(CAN_Inputs_Vector_Mutex);

		/* set safety-out pin */
		SPI_outputs_vector.cvc_err = 1;
		SPI_outputs_vector.cvc_warn = 0;

		if (CAN_inputs[DASH_REVERSE_BUTTON] != 0) {
			drive_state = REVERSE;
		} else if (CAN_inputs[DASH_REVERSE_BUTTON] != 0) {
			drive_state = REVERSE;
		} else if (CAN_inputs[DASH_REVERSE_BUTTON] != 0) {
			drive_state = REVERSE;
		} else {
			drive_state = NEUTRAL;
		}

		/* get push button state */
		xSemaphoreTake(SPI_Inputs_Vector_Mutex, portMAX_DELAY);

		push_button = SPI_inputs_vector.Ready_to_drive;
		if (push_button) {
			SPI_outputs_vector.cvc_warn = 1;
		}

		xSemaphoreGive(SPI_Outputs_Vector_Mutex);
		xSemaphoreGive(SPI_Inputs_Vector_Mutex);

		/* set SPI outputs */

//		cvc_state = DRIVE;
//
//		pack_voltage = (float) 50.0;
//		bus_voltage = (float) ((int)CAN_inputs[DC_BUS_VOLTAGE])/10.0;
//
//		/* check for open Tractive System Master Switch */
//		if (bus_voltage <= 0.9 * pack_voltage)
//		{
//			cvc_state = PRECHARGE;
//		}


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

cvc_state_t get_cvc_state() {
	return cvc_state;
}

cvc_state_t get_drive_state() {
	return drive_state;
}

cvc_fault_status_t get_cvc_fault() {
	return cvc_fault;
}
