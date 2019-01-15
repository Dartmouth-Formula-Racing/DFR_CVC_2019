/*
 * cvc_can.c
 *
 *  Created on: Nov 30, 2018
 *      Author: f002ccx
 */

/* Includes ------------------------------------------------------------------------*/
#include "cvc_can.h"
#include "FreeRTOS.h"
#include "task.h"
#include "demo.h"
#include "queue.h"

/* Private TypeDefs ---------------------------------------------------------------*/
/* Struct to hold messages used in CAN message queues */
typedef struct queue_msg_s
{
	union
	{
		CAN_TxHeaderTypeDef	Tx_header;
		CAN_RxHeaderTypeDef	Rx_header;
	};
	CAN_data_t data;
} queue_msg_t;

/* Struct to hold CAN input map */
typedef struct input_map_s
{
	input_index_t index;	/* index in inputs array */
	uint8_t start_byte;		/* input start byte */
	uint8_t start_bit;		/* input start bit */
	uint8_t size;			/* input size in bytes */
} input_map_t;

/* Private Variables ---------------------------------------------------------------*/
CAN_HandleTypeDef		CanHandle;
CAN_TxHeaderTypeDef		TxHeader;
CAN_RxHeaderTypeDef		RxHeader;
uint8_t					TxData[8];
uint8_t					RxData[8];
uint32_t 				TxMailbox;

/* CAN message dictionary */
static CAN_msg_t CAN_dict[]	=
{
		// ATHENA ECU MSGS messages
		{0x200, STD, 0, 0, "ATHENA1"}, //ATHENA 1 (0)
		{0x310, STD, 0, 0, "ATHENA2"}, //ATHENA 1 (1)
		{0x312, STD, 0, 0, "ATHENA3"}, //ATHENA 1 (2)

		// EMUS BMS messages
		{0x1B6, STD, 0, 0, "EMUS1"}, // EMUS BMS (3)
		{0x1B7, STD, 0, 0, "EMUS2"}, // EMSU BMS (4)
		{0x1BA, STD, 0, 0, "EMUS3"}, // EMSU BMS (5)

		// BAMOCAR messages
		{0x180, STD, 0x30, 0, "BAMO1"}, // BAMOCAR 1 - Motor RPM (6)
		{0x180, STD, 0x5F, 0, "BAMO2"}, // BAMOCAR 2 - Motor Current // change to reg 27..? (7)
		{0x180, STD, 0xA0, 0, "BAMO3"}, // BAMOCAR 3 - Motor Torque (8)
		{0x180, STD, 0x8A, 0, "BAMO4"}, // BAMOCAR 4 - Motor Voltage (9)
		{0x180, STD, 0xE1, 0, "BAMO5"}, // BAMOCAR 5 - BAMOCAR D_OUT2 (10)
		{0x180, STD, 0x8F, 0, "BAMO6"}, // BAMOCAR 6 - BAMOCAR_Fault (11)
		{0x180, STD, 0xEB, 0, "BAMO7"}, // BAMOCAR 7 - BAMOCAR Bus Voltage (12)
		{0x180, STD, 0xE0, 0, "BAMO8"}, // BAMOCAR 8 - BAMOCAR D_OUT 1 (13)
};

/* CAN message input maps */
input_map_t ATHENA1_map =
{
		{ENG_REV_COUNT, 0, 0, 2},
		{ENG_RPM, 2, 0, 2},
		{ENG_TPS, 4, 0, 1},
		{ENG_MAP, 6, 0, 2}
};

input_map_t ATHENA2_map =
{
		{ENG_TEMP, 0, 0, 1},
		{AIR_TEMP, 1, 0, 1},
		{OIL_TEMP, 2, 0, 1},
		{KL15_BATT_VOLTAGE, 3, 0, 1},
		{KL30_BATT_VOLTAGE, 4, 0, 1},
		{BARO, 6, 0, 2},
};

input_map_t ATHENA3_map =
{
		{FLAG_OVERHEAT, 4, 5, 1},
		{ACTIVE_MAP, 5, 0, 1},
};

input_map_t EMUS1_map =
{
		{MIN_CELL_VOLTAGE, 0, 0, 1},
		{MAX_CELL_VOLTAGE, 1, 0, 1},
		{AVG_CELL_VOLTAGE, 2, 0, 1},
		{TOTAL_VOLTAGE, 3, 0, 4},		// bytes in the following order: 5, 3, 6, 4
};

input_map_t EMUS2_map =
{
		{MIN_CELL_TEMP, 0, 0, 1},
		{MAX_CELL_TEMP, 1, 0, 1},
		{AVG_CELL_TEMP, 2, 0, 1},
};

input_map_t EMUS3_map =
{
		{BATT_CURRENT, 0, 0, 2},
		{BATT_CHARGE, 2, 0, 2},
		{BATT_SOC, 6, 0, 1},
};

static CAN_msg_t		demo_message;	// CAN message received through demo

static QueueHandle_t RxQueue = NULL;
static QueueHandle_t TxQueue = NULL;
//static QueueHandle_t testQueue = NULL;

/* Private Function Prototypes ---------------------------------------------------------------*/
static void CAN_Config(void);

/* Task Functions ---------------------------------------------------------------*/
/**
 * @brief 	Reads CAN messages from data table and blinks corresponding LED, sends CAN messages to CAN_Tx_Task
 */
void CAN_Demo_Task(void * parameters)
{
	uint8_t LED_send = 0U;
	queue_msg_t Tx_msg;

	while (1)
	{
		/* Delay task for 1 seconds */
		vTaskDelay((TickType_t) 1000/portTICK_PERIOD_MS);

		/* Build CAN message */
		Tx_msg.Tx_header = TxHeader;
		Tx_msg.data._8[0] = LED_send;
		Tx_msg.data._8[1] = 0xAD;

		/* Add CAN message to TxQueue */
		xQueueSend(TxQueue, &Tx_msg, 0U);

		/* Increment LED_send */
		LED_send = (LED_send % 3)+1;

		/* send to test queue */
		//xQueueSend(testQueue, &LED_send, 0U);

		/* Read received data from data table and turn on corresponding LED */
		LED_Display(demo_message.data._8[0]);
	}

}

/**
 * @brief	Reads incoming CAN messages from RxQueue and adds to data table
 *
 */
void CAN_Rx_Task(void * parameters)
{
	queue_msg_t Rx_msg;

	while(1)
	{
		/* get message from queue */
		xQueueReceive( RxQueue, &Rx_msg, portMAX_DELAY );

		/* filter messages */
		uint8_t done = 0;
		uint8_t i = 0;
		while(i < sizeof(CAN_dict)/sizeof(CAN_msg_t) && !done)
		{
			if (Rx_msg.Rx_header.StdId == CAN_dict[i].msg_ID)
			{
				if ((Rx_msg.Rx_header.StdId == 0x180) && (Rx_msg.data._8[0] == CAN_dict[i].reg_ID))
				{
					// process as Bamocar message
					done = 1;
				}
				else
				{
					// process as regular (not Bamocar) message
					done = 1;
				}
			}
			i++;
		}

		/* store message in data table */
		demo_message.data = Rx_msg.data;
		demo_message.msg_ID = Rx_msg.Rx_header.StdId;
		demo_message.msg_type = Rx_msg.Rx_header.IDE;
	}
}

/**
 * @brief 	Reads outgoing CAN messages from TxQueue and sends to CAN bus
 */
void CAN_Tx_Task(void * parameters)
{
	queue_msg_t Tx_msg;
	while(1)
	{
		/* Read all messages from queue and send to bus */
		//while (uxQueueMessagesWaiting(TxQueue) > 0)
		//{
			/* get message from test queue */
			uint8_t LED_receive;
			//xQueueReceive(testQueue, &LED_receive, portMAX_DELAY);
			//LED_Display(LED_receive);

			/* get message from queue */
			xQueueReceive( TxQueue, &Tx_msg, portMAX_DELAY );

			if (HAL_CAN_AddTxMessage(&CanHandle, &Tx_msg.Tx_header, Tx_msg.data._8, &TxMailbox) != HAL_OK)
			{
				/* Transmission request error */
			}

		//}
	}
}

/* Non-Task Functions ---------------------------------------------------------------*/

/**
 * @brief	Initialize CAN
 */
void CAN_Init(void)
{
	/* Configure the CAN peripheral */
	CAN_Config();

	/* Initialize Tx and Rx Queues */
	RxQueue = xQueueCreate(CAN_Rx_QUEUE_LENGTH, sizeof(queue_msg_t));
	if (RxQueue == NULL)
	{
		Error_Handler();
	}

	TxQueue = xQueueCreate(CAN_Tx_QUEUE_LENGTH, sizeof(queue_msg_t));
	if (TxQueue == NULL)
	{
		Error_Handler();
	}

	//testQueue = xQueueCreate(1U, sizeof(uint8_t));

}


/**
  * @brief		Configures the CAN
  * @param		None
  * @retval		None
  */
static void CAN_Config(void)
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
  }

  /* Add message to RxQueue */
  queue_msg_t Rx_msg;
  Rx_msg.Rx_header = RxHeader;
  for (int i=0; i<sizeof(RxData); i++)	{
	  Rx_msg.data._8[i] = RxData[i];
  }
  xQueueSendFromISR(RxQueue, &Rx_msg, NULL);

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

  /*##-3- Configure the NVIC (Interrupt) #################################################*/
  /* NVIC configuration for CAN1 Reception complete interrupt */
  HAL_NVIC_SetPriority(CANx_RX_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
  HAL_NVIC_EnableIRQ(CANx_RX_IRQn);
}

