/*
 * cvc_spi.c
 *
 *  Created on: Jan 9, 2019
 *      Author: f002ccx
 */

/* Includes ------------------------------------------------------------------*/
#include "cvc_spi.h"

/* Defines -------------------------------------------------------------------*/
#define		SPI_BUFFER_SIZE 	5

/* Uncomment this line to use the board as master, if not it is used as slave */
//#define MASTER_BOARD

/* Buffer used for transmission */
uint8_t aTxBuffer[] = "**** SPI_TwoBoards_FullDuplex_IT communication **** SPI_TwoBoards_FullDuplex_IT communication **** SPI_TwoBoards_FullDuplex_IT communication ****";
uint8_t ubNbDataToTransmit = sizeof(aTxBuffer);
__IO uint8_t ubTransmitIndex = 0;

/* Buffer used for reception */
uint16_t aRxBuffer[sizeof(aTxBuffer)];
uint8_t ubNbDataToReceive = sizeof(aTxBuffer);
__IO uint8_t ubReceiveIndex = 0;


/* Temporary function prototypes -------------------------------------------------*/
void LED_Blinking(uint32_t Period);

void LED_Off(void);



/* Temporary Variables -----------------------------------------------------------*/
__IO uint8_t ubButtonPress = 0;

/* External Variables ------------------------------------------------------------*/
volatile CLT_Read_u_t			CLT_Read;
volatile uint16_t				CLT_Write;

volatile VNI_Read_u_t			VNI_Read;
volatile VNI_Write_u_t			VNI_Write;

volatile SPI_io_states_t		SPI_io_state = wait_for_next_transmission;

volatile SPI_inputs_vector_t	SPI_inputs_vector;
volatile SPI_outputs_vector_t	SPI_outputs_vector;


/* Functions ---------------------------------------------------------------------*/

/**
 *
 *
 */
void initiate_SPI_transmission(void)
{
	LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_15);
	LL_SPI_TransmitData16(SPI1, CLT_Write);
	SPI_io_state = wait_for_CLT;
}

volatile uint32_t SPI1_SR = 0;

void SPI_routine(void)
{
	switch(SPI_io_state)
	{
		case(wait_for_CLT):
			LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_15);

			add_to_SPI_input_buffer((uint16_t)SPI1->DR);

			SPI1_SR = SPI1->SR;

			CLT_Read = debounce_SPI_input();

			SPI_PLC_Set_Inputs();

			SPI_PLC_Set_Outputs();


			LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_14);
			LL_SPI_TransmitData16(SPI1, VNI_Write.word);
			SPI_io_state = wait_for_VNI;
			break;
		case(wait_for_VNI):
			LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_14);

			VNI_Read.word = SPI1->DR;
			SPI1_SR = SPI1->SR;

			SPI_io_state = wait_for_next_transmission;
			break;
		default:
			break;
	};

}


int SPI_buffer_index = 0;

CLT_Read_u_t CLT_read_buffer[SPI_BUFFER_SIZE] = {0};

void add_to_SPI_input_buffer(uint16_t new_data)
{
	CLT_read_buffer[SPI_buffer_index].word = new_data;
	SPI_buffer_index++;

	if(SPI_buffer_index >= SPI_BUFFER_SIZE)
	{
		SPI_buffer_index = 0;
	}
}


int i;

CLT_Read_u_t all_ones;
CLT_Read_u_t all_zeroes;
CLT_Read_u_t Or_temp;
CLT_Read_u_t debounced_data = {0};

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
	SPI_inputs_vector.ICE_enable 					= CLT_Read.bit.IN1;
	SPI_inputs_vector.Motor_enable 					= CLT_Read.bit.IN2;
	SPI_inputs_vector.Ready_to_drive 				= CLT_Read.bit.IN3;
	SPI_inputs_vector.Dash_BRB_press 				= !CLT_Read.bit.IN4;
	SPI_inputs_vector.IMD_safety_circuit_fault 		= CLT_Read.bit.IN5;
	SPI_inputs_vector.BMS_safety_circuit_fault 		= CLT_Read.bit.IN6;
	SPI_inputs_vector.Bamocar_safety_circuit_fault 	= CLT_Read.bit.IN7;

}


/**
  * @brief	Set PLC_Write values using SPI_ouputs_vector values
  * @param	None
  * @retval	None
  */
void SPI_PLC_Set_Outputs(void)
{
	VNI_Write.bit.IN1 = SPI_outputs_vector.safety;
	VNI_Write.bit.IN2 = SPI_outputs_vector.ready_to_drive;
	VNI_Write.bit.IN3 = SPI_outputs_vector.rfg;
	VNI_Write.bit.IN4 = SPI_outputs_vector.ignition_kill;
	VNI_Write.bit.IN5 = SPI_outputs_vector.downshift_solenoid;
	VNI_Write.bit.IN6 = SPI_outputs_vector.upshift_solenoid;
	VNI_Write.bit.IN7 = 0;
	VNI_Write.bit.IN8 = 0;

	set_SPI_check_bit_outputs(&VNI_Write);
}


/**
  * @brief	Set special output bits for PLC_Write
  * @param	None
  * @retval	None
  */
void set_SPI_check_bit_outputs(volatile VNI_Write_u_t *WriteValue)
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
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE);

  /* Configure SCK Pin connected to pin 10 of CN7 connector */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_5, LL_GPIO_AF_5);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_MEDIUM);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_5, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_5, LL_GPIO_OUTPUT_PUSHPULL);

  /* Configure MISO Pin connected to pin 12 of CN7 connector */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_6, LL_GPIO_AF_5);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_MEDIUM);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_6, LL_GPIO_PULL_UP);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_6, LL_GPIO_OUTPUT_OPENDRAIN);

  /* Configure MOSI Pin connected to pin 14 of CN7 connector */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_7, LL_GPIO_AF_5);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_MEDIUM);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_7, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_7, LL_GPIO_OUTPUT_PUSHPULL);


  LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_5);
  LL_GPIO_SetOutputPin(GPIOA, GPIO_PIN_7);


  /* Configure SPI_CS1 PD15 CLT01-38SQ7 */
  LL_GPIO_SetPinMode(GPIOD, LL_GPIO_PIN_15, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinSpeed(GPIOD, LL_GPIO_PIN_15, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinPull(GPIOD, LL_GPIO_PIN_15, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinOutputType(GPIOD, LL_GPIO_PIN_15, LL_GPIO_OUTPUT_PUSHPULL);

  /* Configure SPI_CS2 PD14 VNI8200XP */
  LL_GPIO_SetPinMode(GPIOD, LL_GPIO_PIN_14, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinSpeed(GPIOD, LL_GPIO_PIN_14, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinPull(GPIOD, LL_GPIO_PIN_14, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinOutputType(GPIOD, LL_GPIO_PIN_14, LL_GPIO_OUTPUT_PUSHPULL);

  /* Configure OUT_EN PE9 VNI8200XP */
  LL_GPIO_SetPinMode(GPIOE, LL_GPIO_PIN_9, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinSpeed(GPIOE, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinPull(GPIOE, LL_GPIO_PIN_9, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinOutputType(GPIOE, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_PUSHPULL);


  LL_GPIO_SetOutputPin(GPIOD, GPIO_PIN_15);
  LL_GPIO_SetOutputPin(GPIOD, GPIO_PIN_14);
  LL_GPIO_SetOutputPin(GPIOE, GPIO_PIN_9);

  /* (2) Configure NVIC for SPI1 transfer complete/error interrupts **********/
  /* Set priority for SPI1_IRQn */
  NVIC_SetPriority(SPI1_IRQn, 0);
  /* Enable SPI1_IRQn           */
  NVIC_EnableIRQ(SPI1_IRQn);

  /* (3) Configure SPI1 functional parameters ********************************/

  /* Enable the peripheral clock of GPIOA */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

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
  /* Enable TXE   Interrupt             */
  //LL_SPI_EnableIT_TXE(SPI1);
  /* Enable Error Interrupt             */
  //LL_SPI_EnableIT_ERR(SPI1);
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
}



/**
  * @brief  Wait end of transfer and check if received Data are well.
  * @param  None
  * @retval None
  */
void WaitAndCheckEndOfTransfer(void)
{
  /* 1 - Wait end of transmission */
  while (ubTransmitIndex != ubNbDataToTransmit)
  {
  }
  /* Disable TXE Interrupt */
  LL_SPI_DisableIT_TXE(SPI1);

  /* 2 - Wait end of reception */
  while (ubNbDataToReceive > ubReceiveIndex)
  {
  }
  /* Disable RXNE Interrupt */
  LL_SPI_DisableIT_RXNE(SPI1);

  /* 3 - Compare Transmit data to receive data */
  if(Buffercmp8((uint8_t*)aTxBuffer, (uint8_t*)aRxBuffer, ubNbDataToTransmit))
  {
    /* Processing Error */
    LED_Blinking(LED_BLINK_ERROR);
  }
  else
  {
    /* Turn On Led if data are well received */
	LL_GPIO_SetOutputPin(LED1_GPIO_PORT, LED1_PIN);
  }
}

/**
* @brief Compares two 8-bit buffers and returns the comparison result.
* @param pBuffer1: pointer to the source buffer to be compared to.
* @param pBuffer2: pointer to the second source buffer to be compared to the first.
* @param BufferLength: buffer's length.
* @retval   0: Comparison is OK (the two Buffers are identical)
*           Value different from 0: Comparison is NOK (Buffers are different)
*/
uint8_t Buffercmp8(uint8_t* pBuffer1, uint8_t* pBuffer2, uint8_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}



/******************************************************************************/
/*   USER IRQ HANDLER TREATMENT Functions                                     */
/******************************************************************************/

/**
  * @brief  Function called from SPI1 IRQ Handler when RXNE flag is set
  *         Function is in charge of retrieving received byte from SPI lines.
  * @param  None
  * @retval None
  */
void  SPI1_Rx_Callback(void)
{
  /* Read character in Data register.
  RXNE flag is cleared by reading data in DR register */
  aRxBuffer[ubReceiveIndex++] = LL_SPI_ReceiveData16(SPI1);
}

/**
  * @brief  Function called from SPI1 IRQ Handler when TXE flag is set
  *         Function is in charge  to transmit byte on SPI lines.
  * @param  None
  * @retval None
  */
void  SPI1_Tx_Callback(void)
{
  /* Write character in Data register.
  TXE flag is cleared by reading data in DR register */
  LL_SPI_TransmitData8(SPI1, aTxBuffer[ubTransmitIndex++]);
}

/**
  * @brief  Function called in case of error detected in SPI IT Handler
  * @param  None
  * @retval None
  */
void SPI1_TransferError_Callback(void)
{
  /* Disable RXNE  Interrupt             */
  LL_SPI_DisableIT_RXNE(SPI1);

  /* Disable TXE   Interrupt             */
  LL_SPI_DisableIT_TXE(SPI1);

  /* Set LED1 to Blinking mode to indicate error occurs */
  LED_Blinking(LED_BLINK_ERROR);
}


/**
  * @brief  Function to manage User push-button
  * @param  None
  * @retval None
  */
void UserButton_Callback(void)
{
  /* Update User push-button variable : to be checked in waiting loop in main program */
  ubButtonPress = 1;
}



/**
  * @brief  Turn-off LED1.
  * @param  None
  * @retval None
  */
void LED_Off(void)
{
  /* Turn LED1 off */
  LL_GPIO_ResetOutputPin(LED1_GPIO_PORT, LED1_PIN);
}


/**
  * @brief  Set LED1 to Blinking mode for an infinite loop (toggle period based on value provided as input parameter).
  * @param  Period : Period of time (in ms) between each toggling of LED
  *   This parameter can be user defined values. Pre-defined values used in that example are :
  *     @arg LED_BLINK_FAST : Fast Blinking
  *     @arg LED_BLINK_SLOW : Slow Blinking
  *     @arg LED_BLINK_ERROR : Error specific Blinking
  * @retval None
  */
void LED_Blinking(uint32_t Period)
{
  /* Toggle LED1 in an infinite loop */
  while (1)
  {
    LL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
    LL_mDelay(Period);
  }
}

/**
  * @brief  Initialize LED1.
  * @param  None
  * @retval None
  */
void LED_Init(void)
{
  /* Enable the LED1 Clock */
  LED1_GPIO_CLK_ENABLE();

  /* Configure IO in output push-pull mode to drive external LED1 */
  LL_GPIO_SetPinMode(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_MODE_OUTPUT);
  /* Reset value is LL_GPIO_OUTPUT_PUSHPULL */
  //LL_GPIO_SetPinOutputType(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  /* Reset value is LL_GPIO_SPEED_FREQ_LOW */
  //LL_GPIO_SetPinSpeed(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_SPEED_FREQ_LOW);
  /* Reset value is LL_GPIO_PULL_NO */
  //LL_GPIO_SetPinPull(LED1_GPIO_PORT, LED1_PIN, LL_GPIO_PULL_NO);
}


/**
  * @brief  Configures User push-button in GPIO or EXTI Line Mode.
  * @param  None
  * @retval None
  */
void UserButton_Init(void)
{
  /* Enable the BUTTON Clock */
  USER_BUTTON_GPIO_CLK_ENABLE();

  /* Configure GPIO for BUTTON */
  LL_GPIO_SetPinMode(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, LL_GPIO_PULL_NO);

  /* Connect External Line to the GPIO*/
  USER_BUTTON_SYSCFG_SET_EXTI();

  /* Enable a rising trigger External line 13 Interrupt */
  USER_BUTTON_EXTI_LINE_ENABLE();
  USER_BUTTON_EXTI_FALLING_TRIG_ENABLE();

  /* Configure NVIC for USER_BUTTON_EXTI_IRQn */
  NVIC_EnableIRQ(USER_BUTTON_EXTI_IRQn);
  NVIC_SetPriority(USER_BUTTON_EXTI_IRQn, 0x03);
}

/**
  * @brief  Wait for User push-button press to start transfer.
  * @param  None
  * @retval None
  */
  /*  */
void WaitForUserButtonPress(void)
{
  while (ubButtonPress == 0)
  {
    LL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
    LL_mDelay(LED_BLINK_FAST);
  }
  /* Ensure that LED1 is turned Off */
  LED_Off();
}

