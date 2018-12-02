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


/**
  * @brief		Configures the CAN
  * @param		None
  * @retval		None
  */
void CAN_Config(void)
{
	CAN_FilterTypeDef  sFilterConfig;

	/* 1. Configure the CAN peripheral ---------------------------------------------*/
	CanHandle.Instance = CANx;

	CanHandle.Init.TimeTriggeredMode = DISABLE;
	CanHandle.Init.AutoBusOff = DISABLE;
	CanHandle.Init.AutoWakeUp = DISABLE;
	CanHandle.Init.AutoRetransmission = ENABLE;
	CanHandle.Init.ReceiveFifoLocked= DISABLE;
	CanHandle.Init.TransmitFifoPriority = DISABLE;
	CanHandle.Init.Mode = CAN_MODE_NORMAL;
	CanHandle.Init.SyncJumpWidth = CAN_SJW_1TQ;
	CanHandle.Init.TimeSeg1 = CAN_BS1_6TQ;
	CanHandle.Init.TimeSeg2 = CAN_BS2_2TQ;
	CanHandle.Init.Prescaler = 6;

	if (HAL_CAN_Init(&CanHandle) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}

	/* 2. Configure the CAN Filer ----------------------------------------------------*/
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;

	if (HAL_CAN_ConfigFilter(&CanHandle, &sFilterConfig) != HAL_OK)
	{
		/* Filter Configuration Error */
		Error_Handler();
	}


	/* 3. Start the CAN peripheral ---------------------------------------------------*/
	if (HAL_CAN_Start(&CanHandle) != HAL_OK)
	{
		/* Start Error */
		Error_Handler();
	}

	/* 4. Activate CAN RX notification -----------------------------------------------*/
	if (HAL_CAN_ActivateNotification(&CanHandle, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
		/* Notification Error */
		Error_Handler();
	}

	/* 5. Configure Transmission Process----------------------------------------------*/
	TxHeader.StdId = 0x321;
	TxHeader.ExtId= 0x01;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 2;
	TxHeader.TransmitGlobalTime = DISABLE;
}
