/*
 * cvc_can.c
 *
 *  Created on: Nov 30, 2018
 *      Author: f002ccx
 */

/* Includes ------------------------------------------------------------------------*/
#include "cvc_can.h"


/* Private Variables ---------------------------------------------------------------*/
CAN_HandleTypeDef		CanHandle;
CAN_TxHeaderTypeDef		TxHeader;
CAN_RxHeaderTypeDef		RxHeader;
uint8_t					TxData[8];
uint8_t					RxData[8];
uint32_t 				TxMailbox;



void CAN_Config(void)
{
	CAN_FilterTypeDef  sFilterConfig;

	CanHandle.Instance = CANx;



}
