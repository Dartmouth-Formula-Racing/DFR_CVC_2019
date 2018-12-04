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
	CanHandle.Init.TimeSeg1 = CAN_BS1_7TQ;
	CanHandle.Init.TimeSeg2 = CAN_BS2_2TQ;
	CanHandle.Init.Prescaler = 9;

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


void demo_transmit_func(uint8_t ubKeyNumber)
{
	/* Set the data to be transmitted */
	TxData[0] = ubKeyNumber;
	TxData[1] = 0xAD;

	/* Start Transmission process */
	if (HAL_CAN_AddTxMessage(&CanHandle, &TxHeader, TxData, &TxMailbox) != HAL_OK)
	{
		/* Transmission request error */
		Error_Handler();
	}
}



/**
  * @brief  Rx Fifo 0 message pending callback
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  /* Get RX message */
  if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }

  /* Display LEDx */
  if ((RxHeader.StdId == 0x321) && (RxHeader.IDE == CAN_ID_STD) && (RxHeader.DLC == 2))
  {
	Special_LED_Disp(RxData);
  }
}



/**
  * @brief  Configures the CAN, transmit and receive by polling
  * @param  None
  * @retval PASSED if the reception is well done, FAILED in other case
  */
HAL_StatusTypeDef CAN_Polling(void)
{
  CAN_FilterTypeDef  sFilterConfig;

  /*##-1- Configure the CAN peripheral #######################################*/
  CanHandle.Instance = CANx;

  CanHandle.Init.TimeTriggeredMode = DISABLE;
  CanHandle.Init.AutoBusOff = DISABLE;
  CanHandle.Init.AutoWakeUp = DISABLE;
  CanHandle.Init.AutoRetransmission = ENABLE;
  CanHandle.Init.ReceiveFifoLocked = DISABLE;
  CanHandle.Init.TransmitFifoPriority = DISABLE;
  CanHandle.Init.Mode = CAN_MODE_LOOPBACK;
  CanHandle.Init.SyncJumpWidth = CAN_SJW_1TQ;
  CanHandle.Init.TimeSeg1 = CAN_BS1_6TQ;
  CanHandle.Init.TimeSeg2 = CAN_BS2_2TQ;
  CanHandle.Init.Prescaler = 6;

  if(HAL_CAN_Init(&CanHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  /*##-2- Configure the CAN Filter ###########################################*/
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

  if(HAL_CAN_ConfigFilter(&CanHandle, &sFilterConfig) != HAL_OK)
  {
    /* Filter configuration Error */
    Error_Handler();
  }
  /*##-3- Start the CAN peripheral ###########################################*/
  if (HAL_CAN_Start(&CanHandle) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
  /*##-4- Start the Transmission process #####################################*/
  TxHeader.StdId = 0x11;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.DLC = 2;
  TxHeader.TransmitGlobalTime = DISABLE;
  TxData[0] = 0xCA;
  TxData[1] = 0xFE;

  /* Request transmission */
  if(HAL_CAN_AddTxMessage(&CanHandle, &TxHeader, TxData, &TxMailbox) != HAL_OK)
  {
    /* Transmission request Error */
    Error_Handler();
  }

  /* Wait transmission complete */
  while(HAL_CAN_GetTxMailboxesFreeLevel(&CanHandle) != 3) {}
  /*##-5- Start the Reception process ########################################*/
  if(HAL_CAN_GetRxFifoFillLevel(&CanHandle, CAN_RX_FIFO0) != 1)
  {
    /* Reception Missing */
    Error_Handler();
  }
  if(HAL_CAN_GetRxMessage(&CanHandle, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }
  if((RxHeader.StdId != 0x11)                     ||
     (RxHeader.RTR != CAN_RTR_DATA)               ||
     (RxHeader.IDE != CAN_ID_STD)                 ||
     (RxHeader.DLC != 2)                          ||
     ((RxData[0]<<8 | RxData[1]) != 0xCAFE))
  {
    /* Rx message Error */
    return HAL_ERROR;
  }
  return HAL_OK; /* Test Passed */
}




/**
  * @brief	This function is executed in the case of an error
  * @param	None
  * @retval	None
  */
void Error_Handler(void)
{
	while(1)
	{
	}
}



/**
  * @brief CAN MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - NVIC configuration for DMA interrupt request enable
  * @param hcan: CAN handle pointer
  * @retval None
  */
void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
  GPIO_InitTypeDef   GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* CAN1 Periph clock enable */
  CANx_CLK_ENABLE();
  /* Enable GPIO clock ****************************************/
  CANx_GPIO_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* CAN1 TX GPIO pin configuration */
  GPIO_InitStruct.Pin = CANx_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Alternate =  CANx_TX_AF;

  HAL_GPIO_Init(CANx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* CAN1 RX GPIO pin configuration */
  GPIO_InitStruct.Pin = CANx_RX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Alternate =  CANx_RX_AF;

  HAL_GPIO_Init(CANx_RX_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the NVIC #################################################*/
  /* NVIC configuration for CAN1 Reception complete interrupt */
  HAL_NVIC_SetPriority(CANx_RX_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(CANx_RX_IRQn);
}

