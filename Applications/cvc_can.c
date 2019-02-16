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
#include "semphr.h"
#include "bamocar.h"
#include "cvc_state_machine.h"



/* Private Function Prototypes ---------------------------------------------------------------*/
static void CAN_Config(void);
static void CAN_parser_std(queue_msg_t q_msg, uint8_t CAN_idx);
static void CAN_parser_EMUS1(queue_msg_t q_msg, uint8_t CAN_idx);
static void CAN_parser_BAMO(queue_msg_t q_msg, uint8_t CAN_idx);


/* Private Variables ---------------------------------------------------------------*/
CAN_HandleTypeDef		CanHandle;
uint32_t 				TxMailbox;


/* CAN message input maps */
/* INDEX, START BYTE, START BIT, BYTE SIZE */
input_map_t ATHENA1_map[] =
{
		{ENG_REV_COUNT, 0, 0, 2},
		{ENG_RPM, 2, 0, 2},
		{ENG_TPS, 4, 0, 1},
		{ENG_MAP, 6, 0, 2}
};

input_map_t ATHENA2_map[] =
{
		{ENG_TEMP, 0, 0, 1},
		{AIR_TEMP, 1, 0, 1},
		{OIL_TEMP, 2, 0, 1},
		{KL15_BATT_VOLTAGE, 3, 0, 1},
		{KL30_BATT_VOLTAGE, 4, 0, 1},
		{BARO, 6, 0, 2},
};

input_map_t ATHENA3_map[] =
{
		{FLAG_OVERHEAT, 4, 5, 1},
		{ACTIVE_MAP, 5, 0, 1},
};

input_map_t EMUS2_map[] =
{
		{MIN_CELL_TEMP, 0, 0, 1},
		{MAX_CELL_TEMP, 1, 0, 1},
		{AVG_CELL_TEMP, 2, 0, 1},
};

input_map_t EMUS3_map[] =
{
		{BATT_CURRENT, 0, 0, 2},
		{BATT_CHARGE, 2, 0, 2},
		{BATT_SOC, 6, 0, 1},
};


/* CAN message dictionary */
static CAN_msg_t CAN_dict[]	=
{
		// ATHENA ECU MSGS messages
		{0x200, STD, 0, 0, "ATHENA1", ATHENA1_map, 4, CAN_parser_std},	// ATHENA 1 (0)
		{0x310, STD, 0, 0, "ATHENA2", ATHENA2_map, 6, CAN_parser_std},	// ATHENA 2 (1)
		{0x312, STD, 0, 0, "ATHENA3", ATHENA3_map, 2, CAN_parser_std},	// ATHENA 3 (2)

		// EMUS BMS messages
		{0x1B6, STD, 0, 0, "EMUS1", NULL, 0, CAN_parser_EMUS1}, 		// EMUS BMS 1 (3)
		{0x1B7, STD, 0, 0, "EMUS2", EMUS2_map, 3, CAN_parser_std}, 		// EMSU BMS 2 (4)
		{0x1BA, STD, 0, 0, "EMUS3", EMUS3_map, 3, CAN_parser_std}, 		// EMSU BMS 3 (5)

		// BAMOCAR messages
		{0x180, STD, MOTOR_RPM_REGID, 0, "BAMO1", NULL, 0, CAN_parser_BAMO}, 			// BAMOCAR 1 - Motor RPM (6)
		{0x180, STD, MOTOR_CURRENT_REGID, 0, "BAMO2", NULL, 0, CAN_parser_BAMO}, 		// BAMOCAR 2 - Motor Current (7)
		{0x180, STD, MOTOR_TORQUE_REGID, 0, "BAMO3", NULL, 0, CAN_parser_BAMO}, 		// BAMOCAR 3 - Motor Torque (8)
		{0x180, STD, MOTOR_VOLTAGE_REGID, 0, "BAMO4", NULL, 0, CAN_parser_BAMO}, 		// BAMOCAR 4 - Motor Voltage (9)
		{0x180, STD, BAMOCAR_D_OUT_2_REGID, 0, "BAMO5", NULL, 0, CAN_parser_BAMO}, 		// BAMOCAR 5 - BAMOCAR D_OUT2 (10)
		{0x180, STD, BAMOCAR_FAULT_REGID, 0, "BAMO6", NULL, 0, CAN_parser_BAMO}, 		// BAMOCAR 6 - BAMOCAR_Fault (11)
		{0x180, STD, BAMOCAR_BUS_VOLTAGE_REGID, 0, "BAMO7", NULL, 0, CAN_parser_BAMO}, 	// BAMOCAR 7 - BAMOCAR Bus Voltage (12)
		{0x180, STD, BAMOCAR_D_OUT_1_REGID, 0, "BAMO8", NULL, 0, CAN_parser_BAMO}, 		// BAMOCAR 8 - BAMOCAR D_OUT 1 (13)
};


/* FreeRTOS Message Queues */
static QueueHandle_t RxQueue = NULL;
static QueueHandle_t TxQueue = NULL;

/* Semaphores ----------------------------------------------------------------*/
volatile SemaphoreHandle_t CAN_Inputs_Vector_Mutex;
volatile SemaphoreHandle_t CAN_Outputs_Vector_Mutex;

/* CAN Inputs Vector ----------------------------------------------------------------*/
volatile uint32_t CAN_inputs[NUM_INPUTS];

/* Task Functions ---------------------------------------------------------------*/

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

		/* search through CAN dictionary until message is found */
		while(i < sizeof(CAN_dict)/sizeof(CAN_msg_t) && !done)
		{
			if (Rx_msg.Rx_header.StdId == CAN_dict[i].msg_ID)
			{
				if (Rx_msg.Rx_header.StdId != 0x180)
				{
					CAN_dict[i].parser(Rx_msg, i);	// call message parser
					done = 1;
				}
				else if (Rx_msg.data._8[0] == CAN_dict[i].reg_ID)
				{
					CAN_dict[i].parser(Rx_msg, i);
					done = 1;
				}
			}
			i++;
		}

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
		/* get message from queue */
		xQueueReceive( TxQueue, &Tx_msg, portMAX_DELAY );

		if (HAL_CAN_AddTxMessage(&CanHandle, &Tx_msg.Tx_header, Tx_msg.data._8, &TxMailbox) != HAL_OK)
		{
			error_handler(CVC_HARD_FAULT, CAN_ERR);
		}
	}
}


/* Non-Task Functions ---------------------------------------------------------------*/

void CAN_Send(queue_msg_t Tx_msg)
{
	/* TODO: check that CAN message is valid */
	if (xQueueSend(TxQueue, &Tx_msg, portMAX_DELAY) != pdPASS)
	{
		error_handler(CVC_HARD_FAULT, QUEUE_ERR);
	}
}

/**
 * @brief standard parser for unpacking CAN functions into CAN_inputs table (big endian)
 * @param q_msg: incoming CAN message
 * @param CAN_msg: reference message from CAN_dict w/ message metadata
 */
static void CAN_parser_std(queue_msg_t q_msg, uint8_t CAN_idx)
{
	/* iterate over all inputs in data field */
	for (int i = 0; i < CAN_dict[CAN_idx].num_inputs; i++)
	{
		uint32_t result = 0;
		input_map_t input = CAN_dict[CAN_idx].input_map[i];


		/* iterate over all bytes of input */
		for (int j = 0; j < input.size; j++)
		{
			result = result << 8 | (uint32_t) (q_msg.data._8[input.start_byte + j] << input.start_bit);
		}

		xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);	//get CAN inputs mutex

		/* store result in CAN_inputs table */
		CAN_inputs[input.index] = result;

		xSemaphoreGive(CAN_Inputs_Vector_Mutex);	//give CAN inputs mutex
	}
}

/**
 * @brief special parser for EMUS1 CAN message
 * @param q_msg: incoming CAN message
 * @param CAN_msg: reference message from CAN_dict w/ message metadata
 */
static void CAN_parser_EMUS1(queue_msg_t q_msg, uint8_t CAN_idx)
{
	xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);	//get CAN inputs mutex

	CAN_inputs[MIN_CELL_VOLTAGE] = q_msg.data._8[0];
	CAN_inputs[MAX_CELL_VOLTAGE] = q_msg.data._8[1];
	CAN_inputs[AVG_CELL_VOLTAGE] = q_msg.data._8[2];
	CAN_inputs[BATT_VOLTAGE] = (uint32_t) q_msg.data._8[5] << 24 | (uint32_t) q_msg.data._8[6] << 16 | (uint32_t) q_msg.data._8[3] << 8 | (uint32_t) q_msg.data._8[4];

	xSemaphoreGive(CAN_Inputs_Vector_Mutex);	//give CAN inputs mutex
}

/**
 * @brief special parser for Bamocar CAN messages
 * @param q_msg: incoming CAN message
 * @param CAN_msg: reference message from CAN_dict w/ message metadata
 */
static void CAN_parser_BAMO(queue_msg_t q_msg, uint8_t CAN_idx)
{
	uint32_t result = 0;

	/* unpack data based on DLC (little endian) */
	if (q_msg.Rx_header.DLC == 0x3 || q_msg.Rx_header.DLC == 0x4)
	{
		/* 2 data bytes */
		result = (uint32_t) q_msg.data._8[2] << 8 | (uint32_t) q_msg.data._8[1];
	}
	else
	{
		/* 4 data bytes */
		result = (uint32_t) q_msg.data._8[4] << 24 | (uint32_t) q_msg.data._8[3] << 16 | (uint32_t) q_msg.data._8[2] << 8 | (uint32_t) q_msg.data._8[1];
	}

	xSemaphoreTake(CAN_Inputs_Vector_Mutex, portMAX_DELAY);	//get CAN inputs mutex

	/* store in CAN_inputs table */
	switch (CAN_dict[CAN_idx].reg_ID)
	{
	case 0x30:
		CAN_inputs[MOTOR_RPM] = result;
		break;
	case 0x5F:
		CAN_inputs[MOTOR_CURRENT] = result;
		break;
	case 0xA0:
		CAN_inputs[MOTOR_TORQUE] = result;
		break;
	case 0x8A:
		CAN_inputs[MOTOR_VOLTAGE] = result;
		break;
	case 0x49:
		CAN_inputs[MOTOR_TEMP] = result;
		break;
	case 0x8F:
		CAN_inputs[BAMO_FAULT] = result;
		break;
	case 0xEB:
		CAN_inputs[BAMO_BUS_VOLTAGE] = result;
		break;
	case 0xE0:
		CAN_inputs[BAMO_D_1_OUT_1] = result;
		break;
	case 0xE1:
		CAN_inputs[BAMO_D_1_OUT_2] = result;
		break;
	default:
		break;
	}

	xSemaphoreGive(CAN_Inputs_Vector_Mutex);	//give CAN inputs mutex
}


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

	/* Initialize CAN Input and Output Vector Mutex's */
	CAN_Inputs_Vector_Mutex = xSemaphoreCreateMutex();
	if (CAN_Inputs_Vector_Mutex == NULL)
	{
		Error_Handler();
	}

	CAN_Outputs_Vector_Mutex = xSemaphoreCreateMutex();
	if (CAN_Outputs_Vector_Mutex == NULL)
	{
		Error_Handler();
	}

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
	CanHandle.Init.TimeSeg1 = CAN_BS1_14TQ;
	CanHandle.Init.TimeSeg2 = CAN_BS2_3TQ;
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

}


/**
  * @brief  Rx Fifo 0 message pending callback
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef		RxHeader;
	uint8_t					RxData[8];

	queue_msg_t Rx_msg;

	/* Get RX message */
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &(RxHeader), RxData) != HAL_OK)
	{
		error_handler(CVC_HARD_FAULT, CAN_ERR);
	}

	/* Add message to RxQueue */
	Rx_msg.Rx_header = RxHeader;
	for (int i=0; i<sizeof(RxData); i++)	{
	  Rx_msg.data._8[i] = RxData[i];
	}
	if (xQueueSendFromISR(RxQueue, &Rx_msg, NULL) != pdPASS)
	{
		error_handler(CVC_HARD_FAULT, QUEUE_ERR);
	}

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

