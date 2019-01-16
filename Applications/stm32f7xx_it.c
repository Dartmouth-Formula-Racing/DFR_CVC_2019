/**
  ******************************************************************************
  * @file    stm32f7xx_it.c
  * @author  Ac6
  * @version V1.0
  * @date    02-Feb-2015
  * @brief   Default Interrupt Service Routines.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
#include "stm32f7xx.h"
#ifdef USE_RTOS_SYSTICK
#include <cmsis_os.h>
#endif
#include "stm32f7xx_it.h"
#include "cvc_can.h"
#include "cvc_spi.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern CAN_HandleTypeDef	CanHandle;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            	  	    Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles SysTick Handler, but only if no RTOS defines it.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
#ifdef USE_RTOS_SYSTICK
	osSystickHandler();
#endif
}

/**
  * @brief	This function handles CAN1 RX0 interrupt requests
  * @param	None
  * @retval	None
  */
void CANx_RX_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&CanHandle);
}




/**
  * @brief  This function handles external line 15_10 interrupt request.
  * @param  None
  * @retval None
  */
void USER_BUTTON_IRQHANDLER(void)
{

  /* Manage Flags */
  if(LL_EXTI_IsActiveFlag_0_31(USER_BUTTON_EXTI_LINE) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(USER_BUTTON_EXTI_LINE);

    /* Manage code in main.c */
    UserButton_Callback();
  }
}

/**
  * @brief  This function handles SPI1 interrupt request.
  * @param  None
  * @retval None
  */
void SPI1_IRQHandler(void)
{
	if(LL_SPI_IsActiveFlag_RXNE(SPI1))
	{
		SPI_routine();
	}

//  /* Check RXNE flag value in ISR register */
// if(LL_SPI_IsActiveFlag_RXNE(SPI1))
//  {
//    /* Call function Slave Reception Callback */
//    SPI1_Rx_Callback();
//  }
//  /* Check RXNE flag value in ISR register */
//  else if(LL_SPI_IsActiveFlag_TXE(SPI1))
//  {
//    /* Call function Slave Reception Callback */
//    SPI1_Tx_Callback();
//  }
//  /* Check STOP flag value in ISR register */
//  else if(LL_SPI_IsActiveFlag_OVR(SPI1))
//  {
//    /* Call Error function */
//    SPI1_TransferError_Callback();
//  }
}
