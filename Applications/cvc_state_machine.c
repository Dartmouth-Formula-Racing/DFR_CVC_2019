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

volatile cvc_state_t cvc_state = PRECHARGE;
//volatile cvc_state_t cvc_state = DRIVE;
volatile cvc_state_t drive_state = DRIVE;
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

static float pack_voltage = 204;
static float bus_voltage_1;
static float bus_voltage_2;
static int undervolt_count = 0;
static int undervolt_timeout = 10;

static uint8_t log_disable_prev = 0xFF;
static uint8_t log_disable_temp = 0xFF;
static uint8_t log_disable_counter = 0;


void state_machine()
{
	// add any checks that must be done in all states here

	/* set safety-out pins */
	xSemaphoreTake(SPI_Outputs_Vector_Mutex, portMAX_DELAY);

	SPI_outputs_vector.CVC_ERROR = 1;
	SPI_outputs_vector.CVC_WARN = 1;

	xSemaphoreGive(SPI_Outputs_Vector_Mutex);

	// states
	switch(cvc_state) {

		case PRECHARGE:

			// make sure 2nd air is off
			xSemaphoreTake(SPI_Outputs_Vector_Mutex, portMAX_DELAY);

			SPI_outputs_vector.AIR_2 = 0;
			SPI_outputs_vector.PUMPS = 0;

			/* get inverter voltages */
			xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);

			bus_voltage_1 = (float) ((int)CAN_inputs[DC_BUS_VOLTAGE])/10.0;	//*96.0f/3600.0f;
			bus_voltage_2 = (float) ((int)CAN_inputs[DC_BUS_VOLTAGE_2])/10.0;

			/* wait for 90% precharge */
			if (bus_voltage_1 >= pack_voltage * 0.9f && bus_voltage_2 >= pack_voltage * 0.9f && !precharge_timer_started)
			{
				precharge_timer = PRE_CHARGE_TIMER_LOAD;
				precharge_timer_started = 1;
			}

			xSemaphoreGive(CAN_Inputs_Vector_Mutex);

			/* start precharge timer and wait */
			if (precharge_timer_started)
			{
				precharge_timer--;
				SPI_outputs_vector.AIR_2 = 0;
			}

			if (precharge_timer_started && precharge_timer <= 0)
			{
				precharge_complete = 1;
			}

			/* close 2nd air and increment state */
			if (precharge_complete)
			{
				cvc_state = DRIVE;
				precharge_timer_started = 0;
				precharge_complete = 0;

				SPI_outputs_vector.AIR_2 = 1;
			}
			else
			{
				cvc_state = PRECHARGE;
			}

			xSemaphoreGive(SPI_Outputs_Vector_Mutex);
		break;

		case DRIVE:

			xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);

			/*if (CAN_inputs[DASH_REVERSE_BUTTON]) {
				drive_state = REVERSE;
			} else if (CAN_inputs[DASH_DRIVE_BUTTON]) {
				drive_state = DRIVE;
			} else if (CAN_inputs[DASH_NEUTRAL_BUTTON]) {
				drive_state = NEUTRAL;
			}*/

			/* get inverter voltages */
			bus_voltage_1 = (float) ((int)CAN_inputs[DC_BUS_VOLTAGE])/10.0;	//*96.0f/3600.0f;
			bus_voltage_2 = (float) ((int)CAN_inputs[DC_BUS_VOLTAGE_2])/10.0;

			/* check for <10% of pack voltage */
			if (bus_voltage_1 <= pack_voltage * 0.1f || bus_voltage_2 <= pack_voltage * 0.1f)
			{
				undervolt_count++;
			}
			else
			{
				undervolt_count = 0;
			}
			if (undervolt_count >= undervolt_timeout)
			{
				cvc_state = PRECHARGE;
			}
			xSemaphoreGive(CAN_Inputs_Vector_Mutex);

			// Turn on cooling pumps
			xSemaphoreTake(SPI_Outputs_Vector_Mutex, portMAX_DELAY);

			SPI_outputs_vector.PUMPS = 1;
			SPI_outputs_vector.AIR_2 = 1;

			xSemaphoreGive(SPI_Outputs_Vector_Mutex);

		break;

		case CHARGING:
			cvc_state = GLV_FAULT;
		break;

		case CHARGE_ERROR:
			cvc_state = GLV_FAULT;
		break;

		case CHARGE_DONE:
			cvc_state = GLV_FAULT;
		break;

		default:
			cvc_state = GLV_FAULT;
		break;
	}
}

void safety_monitor(void)
{
	// haha what safety
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
