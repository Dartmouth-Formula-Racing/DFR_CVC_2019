/*
 * cvc_spi.c
 *
 *  Created on: Jan 9, 2019
 *      Author: f002ccx
 */

/* Includes ------------------------------------------------------------------*/
#include "cvc_spi.h"
#include "cvc_state_machine.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Defines -------------------------------------------------------------------*/
#define		SPI_BUFFER_SIZE 	5

/* Private TypeDefs ---------------------------------------------------------------*/

/* Struct to hold messages used in PLC_transmission_complete queues */
typedef struct PLC_transmission_msg_s
{
	int flag;
} PLC_transmission_msg_t;


/* Free rtos variables -----------------------------------------------------------*/
static QueueHandle_t PLC_transmit_queue = NULL;

/* Semaphores ----------------------------------------------------------------*/
volatile SemaphoreHandle_t SPI_Inputs_Vector_Mutex;
volatile SemaphoreHandle_t SPI_Outputs_Vector_Mutex;

/* External Variables ------------------------------------------------------------*/
volatile CLT_Read_u_t			CLT_Read;
volatile uint16_t				CLT_Write;

volatile VNI_Read_u_t			VNI_Read;
volatile VNI_Write_u_t			VNI_Write;

volatile SPI_io_states_t		SPI_io_state = wait_for_next_transmission;

volatile SPI_inputs_vector_t	SPI_inputs_vector;
volatile SPI_outputs_vector_t	SPI_outputs_vector;


volatile uint32_t SPI1_SR = 0;


int SPI_buffer_index = 0;
CLT_Read_u_t CLT_read_buffer[SPI_BUFFER_SIZE] = {0};


int i;

CLT_Read_u_t all_ones;
CLT_Read_u_t all_zeroes;
CLT_Read_u_t Or_temp;
CLT_Read_u_t debounced_data = {0};


/* Task Functions ----------------------------------------------------------------*/
/**
 * @brief 	Carries out PLC SPI communication routine
 */
void PLC_Routine_Task(void * parameters)
{
	PLC_transmission_msg_t PLC_transmission_message;

	while(1)
		{
			vTaskDelay((TickType_t) 10/portTICK_PERIOD_MS);		// Running at 100 Hz

			xSemaphoreTake(SPI_Outputs_Vector_Mutex, portMAX_DELAY);	//get mutex

			taskENTER_CRITICAL();

			initiate_SPI_transmission();

			taskEXIT_CRITICAL();

			/* get message from queue */
			if (xQueueReceive( PLC_transmit_queue, &PLC_transmission_message, 5/portTICK_PERIOD_MS ) != pdPASS) //change portMAX_DELAY to some # of ticks
			{
				xSemaphoreGive(SPI_Outputs_Vector_Mutex);	//give SPI mutex
				cvc_error_handler(CVC_HARD_FAULT, QUEUE_ERR);
			}
			xSemaphoreGive(SPI_Outputs_Vector_Mutex);	//give SPI mutex
		}
}



/* Functions ---------------------------------------------------------------------*/

/**
  * @brief	When RX Interrupt is received, run SPI_routine()
  * @param	None
  * @retval	None
 */
void PLC_routine_ISR_callback(void)
{
	PLC_transmission_msg_t PLC_transmission_message;

	SPI_routine();

	if ( SPI_io_state == wait_for_next_transmission)
	{
		if (xQueueSendFromISR(PLC_transmit_queue, &PLC_transmission_message, NULL) != pdPASS)
		{
			cvc_error_handler(CVC_HARD_FAULT, QUEUE_ERR);
		}
	}
}


/**
  * @brief	Start SPI communication with PLC board
  * @param	None
  * @retval	None
 */
void initiate_SPI_transmission(void)
{
	/* Begin transmission with PLC by writing CLT_Write (empty) to CLT chip */
	CLT_Write = 0;
	LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_15);
	LL_SPI_TransmitData16(SPI1, CLT_Write);
	SPI_io_state = wait_for_CLT;
}


/**
  * @brief	Conduct SPI data transfers with CLT and VNI chips on the PLC board. Called
  * 			from SPI1_IRQHandler().
  * @param	None
  * @retval	None
 */
void SPI_routine(void)
{
	switch(SPI_io_state)
	{
		case(wait_for_CLT):

			/* Read input pins from PLC (from the CLT chip) into CLT_Read */
			LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_15);
			add_to_SPI_input_buffer(LL_SPI_ReceiveData16(SPI1));
			SPI1_SR = SPI1->SR;
			CLT_Read = debounce_SPI_input();

			SPI_PLC_Set_Inputs();	// Store CLT_Read in SPI inputs vector
			SPI_PLC_Set_Outputs();	// Set values of VNI_Write using outputs vector

			/* Set PLC output pins by transmitting VNI_Write */
			LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_14);
			LL_SPI_TransmitData16(SPI1, VNI_Write.word);

			SPI_io_state = wait_for_VNI;	// Move to wait_for_VNI state
			break;

		case(wait_for_VNI):

			/* Read inputs from the VNI chip */
			LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_14);
			VNI_Read.word = LL_SPI_ReceiveData16(SPI1);
			SPI1_SR = SPI1->SR;

			SPI_io_state = wait_for_next_transmission;	// Move to waiting state
			break;

		default:
			break;
	};
}



/**
  * @brief	Add SPI data to input buffer
  * @param	None
  * @retval	None
 */
void add_to_SPI_input_buffer(uint16_t new_data)
{
	CLT_read_buffer[SPI_buffer_index].word = new_data;
	SPI_buffer_index++;

	if(SPI_buffer_index >= SPI_BUFFER_SIZE)
	{
		SPI_buffer_index = 0;
	}
}


/**
  * @brief	Debounce data from SPI buffer before returning it to CLT_Read
  * @param	None
  * @retval	None
 */
CLT_Read_u_t debounce_SPI_input(void)
{
	all_ones.word = 0xFFFF;
	Or_temp.word = 0;

	for(i=0; i<SPI_BUFFER_SIZE; i++)
	{
		all_ones.word &= CLT_read_buffer[i].word;
		Or_temp.word |= CLT_read_buffer[i].word;
	}
	all_zeroes.word = ~Or_temp.word;

	debounced_data.word = (all_ones.word & (~all_zeroes.word)) | ((~all_zeroes.word) & debounced_data.word);

	return debounced_data;
}


/**
  * @brief	Set SPI_inputs_vector values using PLC_Read data
  * @param	None
  * @retval	None
  */
void SPI_PLC_Set_Inputs(void)
{

#if CVC_PROTOTYPE == 0

	SPI_inputs_vector.ICE_enable 					= CLT_Read.bit.IN3;
	SPI_inputs_vector.Motor_enable 					= CLT_Read.bit.IN4;
	SPI_inputs_vector.Dash_BRB_press 				= CLT_Read.bit.IN5;
	SPI_inputs_vector.Ready_to_drive 				= CLT_Read.bit.IN6;
	SPI_inputs_vector.TSMS_closed					= CLT_Read.bit.IN8;

#endif /* CVC_PROTOTYPE == 0 */
}


/**
  * @brief	Set PLC_Write values using SPI_ouputs_vector values
  * @param	None
  * @retval	None
  */
void SPI_PLC_Set_Outputs(void)
{
#if CVC_PROTOTYPE == 0

	VNI_Write.bit.IN3 = SPI_outputs_vector.safety;
	VNI_Write.bit.IN4 = SPI_outputs_vector.cvc_err;
	VNI_Write.bit.IN5 = SPI_outputs_vector.cvc_warn;
	VNI_Write.bit.IN6 = SPI_outputs_vector.ready_to_drive;
	VNI_Write.bit.IN7 = SPI_outputs_vector.rfg;
	VNI_Write.bit.IN8 = 1;						//ignition kill
	VNI_Write.bit.IN1 = 1;		// ic fan enable
	VNI_Write.bit.IN2 = 0;

#endif /* CVC_PROTOTYPE == 0 */

	set_SPI_parity_check_bit_outputs(&VNI_Write);
}


/**
  * @brief	Set special output bits for PLC_Write
  * @param	None
  * @retval	None
  */
void set_SPI_parity_check_bit_outputs(volatile VNI_Write_u_t *WriteValue)
{
	WriteValue->bit.SPARE= 0;

	WriteValue->bit.P0	=
	(
		  WriteValue->bit.IN1
		+ WriteValue->bit.IN2
		+ WriteValue->bit.IN3
		+ WriteValue->bit.IN4
		+ WriteValue->bit.IN5
		+ WriteValue->bit.IN6
		+ WriteValue->bit.IN7
		+ WriteValue->bit.IN8
	);

	WriteValue->bit.P1 	=
	(
		  WriteValue->bit.IN2
		+ WriteValue->bit.IN4
		+ WriteValue->bit.IN6
		+ WriteValue->bit.IN8
	);

	WriteValue->bit.P2 	=
	(
		  WriteValue->bit.IN1
		+ WriteValue->bit.IN3
		+ WriteValue->bit.IN5
		+ WriteValue->bit.IN7
	);

	WriteValue->bit.nP0 = !(WriteValue->bit.P0);
}


/**
  * @brief  This function configures SPI1.
  * @note  This function is used to :
  *        -1- Enables GPIO clock and configures the SPI1 pins.
  *        -2- Configure NVIC for SPI1.
  *        -3- Configure SPI1 functional parameters.
  * @note   Peripheral configuration is minimal configuration from reset values.
  *         Thus, some useless LL unitary functions calls below are provided as
  *         commented examples - setting is default configuration from reset.
  * @param  None
  * @retval None
  */
void Configure_SPI(void)
{
	/* (1) Enables GPIO clock and configures the SPI1 pins ********************/
	/* Enable the peripheral clock of GPIOA */
	LL_AHB1_GRP1_EnableClock(LL_GPIOA_CLK_ENABLE);
	LL_AHB1_GRP1_EnableClock(LL_GPIOD_CLK_ENABLE);
	LL_AHB1_GRP1_EnableClock(LL_GPIOG_CLK_ENABLE);

#if CVC_PROTOTYPE == 0
	LL_AHB1_GRP1_EnableClock(LL_GPIOB_CLK_ENABLE);
#else
	LL_AHB1_GRP1_EnableClock(LL_GPIOE_CLK_ENABLE);
#endif	/*CVC_PROTOTYPE == 0 */

	/* Configure SCK Pin connected to pin 10 of CN7 connector */
	LL_GPIO_SetPinMode(PLC_SCK_GPIO_PORT, PLC_SCK_PIN, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(PLC_SCK_GPIO_PORT, PLC_SCK_PIN, PLC_SCK_AF);
	LL_GPIO_SetPinSpeed(PLC_SCK_GPIO_PORT, PLC_SCK_PIN, LL_GPIO_SPEED_FREQ_MEDIUM);
	LL_GPIO_SetPinPull(PLC_SCK_GPIO_PORT, PLC_SCK_PIN, LL_GPIO_PULL_DOWN);
	LL_GPIO_SetPinOutputType(PLC_SCK_GPIO_PORT, PLC_SCK_PIN, LL_GPIO_OUTPUT_PUSHPULL);

	/* Configure MISO Pin connected to pin 12 of CN7 connector */
	LL_GPIO_SetPinMode(PLC_MISO_GPIO_PORT, PLC_MISO_PIN, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(PLC_MISO_GPIO_PORT, PLC_MISO_PIN, PLC_MISO_AF);
	LL_GPIO_SetPinSpeed(PLC_MISO_GPIO_PORT, PLC_MISO_PIN, LL_GPIO_SPEED_FREQ_MEDIUM);
	LL_GPIO_SetPinPull(PLC_MISO_GPIO_PORT, PLC_MISO_PIN, LL_GPIO_PULL_UP);
	LL_GPIO_SetPinOutputType(PLC_MISO_GPIO_PORT, PLC_MISO_PIN, LL_GPIO_OUTPUT_OPENDRAIN);

	/* Configure MOSI Pin connected to pin 14 of CN7 connector */
	LL_GPIO_SetPinMode(PLC_MOSI_GPIO_PORT, PLC_MOSI_PIN, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(PLC_MOSI_GPIO_PORT, PLC_MOSI_PIN, PLC_MOSI_AF);
	LL_GPIO_SetPinSpeed(PLC_MOSI_GPIO_PORT, PLC_MOSI_PIN, LL_GPIO_SPEED_FREQ_MEDIUM);
	LL_GPIO_SetPinPull(PLC_MOSI_GPIO_PORT, PLC_MOSI_PIN, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinOutputType(PLC_MOSI_GPIO_PORT, PLC_MOSI_PIN, LL_GPIO_OUTPUT_PUSHPULL);
// Actually PB5 on PCB!


	LL_GPIO_SetOutputPin(PLC_SCK_GPIO_PORT, PLC_SCK_PIN);
	LL_GPIO_SetOutputPin(PLC_MOSI_GPIO_PORT, PLC_MOSI_PIN);


	/* Configure SPI_CS1 PD15 CLT01-38SQ7 */
	LL_GPIO_SetPinMode(PLC_CS1_GPIO_PORT, PLC_CS1_PIN, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinSpeed(PLC_CS1_GPIO_PORT, PLC_CS1_PIN, LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinPull(PLC_CS1_GPIO_PORT, PLC_CS1_PIN, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinOutputType(PLC_CS1_GPIO_PORT, PLC_CS1_PIN, LL_GPIO_OUTPUT_PUSHPULL);

	/* Configure SPI_CS2 PD14 VNI8200XP */
	LL_GPIO_SetPinMode(PLC_CS2_GPIO_PORT, PLC_CS2_PIN, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinSpeed(PLC_CS2_GPIO_PORT, PLC_CS2_PIN, LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinPull(PLC_CS2_GPIO_PORT, PLC_CS2_PIN, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinOutputType(PLC_CS2_GPIO_PORT, PLC_CS2_PIN, LL_GPIO_OUTPUT_PUSHPULL);

	/* Configure OUT_EN PE9 VNI8200XP */
	LL_GPIO_SetPinMode(PLC_OUTEN_GPIO_PORT, PLC_OUTEN_PIN, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinSpeed(PLC_OUTEN_GPIO_PORT, PLC_OUTEN_PIN, LL_GPIO_SPEED_FREQ_LOW);
	LL_GPIO_SetPinPull(PLC_OUTEN_GPIO_PORT, PLC_OUTEN_PIN, LL_GPIO_PULL_NO);
	LL_GPIO_SetPinOutputType(PLC_OUTEN_GPIO_PORT, PLC_OUTEN_PIN, LL_GPIO_OUTPUT_PUSHPULL);


	LL_GPIO_SetOutputPin(PLC_CS1_GPIO_PORT, PLC_CS1_PIN);
	LL_GPIO_SetOutputPin(PLC_CS2_GPIO_PORT, PLC_CS2_PIN);
	LL_GPIO_SetOutputPin(PLC_OUTEN_GPIO_PORT, PLC_OUTEN_PIN);

	/* (2) Configure NVIC for SPI1 transfer complete/error interrupts **********/

	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4); //for free rtos on stm32

	/* Set priority for SPI1_IRQn */
	NVIC_SetPriority(SPI1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
	/* Enable SPI1_IRQn           */
	NVIC_EnableIRQ(SPI1_IRQn);


	/* (3) Configure SPI1 functional parameters ********************************/

	/* Enable the peripheral clock of GPIOA */
	LL_APB2_GRP1_EnableClock(SPI1_PERIPH_CLK_ENABLE);

	/* Configure SPI1 communication */
	LL_SPI_SetBaudRatePrescaler(SPI1, LL_SPI_BAUDRATEPRESCALER_DIV32);
	LL_SPI_SetTransferDirection(SPI1,LL_SPI_FULL_DUPLEX);
	LL_SPI_SetClockPhase(SPI1, LL_SPI_PHASE_1EDGE);			// modified to match John's code
	LL_SPI_SetClockPolarity(SPI1, LL_SPI_POLARITY_LOW);		// modified to match John's code
	/* Reset value is LL_SPI_MSB_FIRST */
	LL_SPI_SetTransferBitOrder(SPI1, LL_SPI_MSB_FIRST);
	LL_SPI_SetDataWidth(SPI1, LL_SPI_DATAWIDTH_16BIT);
	LL_SPI_SetNSSMode(SPI1, LL_SPI_NSS_SOFT);
	LL_SPI_SetRxFIFOThreshold(SPI1, LL_SPI_RX_FIFO_TH_HALF);

	LL_SPI_SetMode(SPI1, LL_SPI_MODE_MASTER);

	/* Configure SPI1 transfer interrupts */
	/* Enable RXNE  Interrupt             */
	LL_SPI_EnableIT_RXNE(SPI1);

}


/**
  * @brief  This function Activate SPI1
  * @param  None
  * @retval None
  */
void Activate_SPI(void)
{
	/* Enable SPI1 */
	LL_SPI_Enable(SPI1);
	PLC_transmit_queue = xQueueCreate(PLC_TRANSMIT_QUEUE_LENGTH, sizeof(PLC_transmission_msg_t));
	if (PLC_transmit_queue == NULL)
	{
		//Error_Handler();
	}

	SPI_Inputs_Vector_Mutex = xSemaphoreCreateMutex();
	if (SPI_Inputs_Vector_Mutex == NULL)
	{
		//Error_Handler();
	}

	SPI_Outputs_Vector_Mutex = xSemaphoreCreateMutex();
	if (SPI_Outputs_Vector_Mutex == NULL)
	{
		//Error_Handler();
	}
}
