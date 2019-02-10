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
#include "stm32f7xx_nucleo_144.h"
#ifdef USE_RTOS_SYSTICK
#include <cmsis_os.h>
#endif
#include "stm32f7xx_it.h"
#include "cvc_serial.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
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

void NMI_Handler(void)
{
	while(1)
	{

	}
}

void prvGetRegistersFromStack( uint32_t *pulFaultStackAddress )
{
/* These are volatile to try and prevent the compiler/linker optimising them
away as the variables never actually get used.  If the debugger won't show the
values of the variables, make them global my moving their declaration outside
of this function. */
volatile uint32_t r0;
volatile uint32_t r1;
volatile uint32_t r2;
volatile uint32_t r3;
volatile uint32_t r12;
volatile uint32_t lr; /* Link register. */
volatile uint32_t pc; /* Program counter. */
volatile uint32_t psr;/* Program status register. */
volatile unsigned long _CFSR ;
volatile unsigned long _HFSR ;
volatile unsigned long _DFSR ;
volatile unsigned long _AFSR ;
volatile unsigned long _BFAR ;
volatile unsigned long _MMAR ;
volatile unsigned char _MMSR;
volatile unsigned char _BFSR;
volatile unsigned char _UFSR;
volatile unsigned char JUNK;

    r0 = pulFaultStackAddress[ 0 ];
    r1 = pulFaultStackAddress[ 1 ];
    r2 = pulFaultStackAddress[ 2 ];
    r3 = pulFaultStackAddress[ 3 ];

    r12 = pulFaultStackAddress[ 4 ];
    lr = pulFaultStackAddress[ 5 ];
    pc = pulFaultStackAddress[ 6 ];
    psr = pulFaultStackAddress[ 7 ];

    // Configurable Fault Status Register
      // Consists of MMSR, BFSR and UFSR
//      _CFSR = (*((volatile unsigned long *)(0xE000ED28))) ;

    _MMSR = (*((volatile unsigned char *)(0xE000ED28))) ;

    _BFSR = (*((volatile unsigned char *)(0xE000ED29))) ;

    _UFSR = (*((volatile unsigned char *)(0xE000ED2A))) ;

    JUNK = (*((volatile unsigned char *)(0xE000ED2B))) ;




//      volatile uint8_t _UFSR = (_CFSR >> 8) & 0xff;
//
//      volatile uint8_t _BFSR = (_CFSR >> 16) & 0xff;
//
//      volatile uint8_t _MMSR = (_CFSR >> 24) & 0xff;


      // Hard Fault Status Register
      _HFSR = (*((volatile unsigned long *)(0xE000ED2C))) ;

      // Debug Fault Status Register
      _DFSR = (*((volatile unsigned long *)(0xE000ED30))) ;

      // Auxiliary Fault Status Register
      _AFSR = (*((volatile unsigned long *)(0xE000ED3C))) ;


      // Read the Fault Address Registers. These may not contain valid values.
      // Check BFARVALID/MMARVALID to see if they are valid values
      // MemManage Fault Address Register
      _MMAR = (*((volatile unsigned long *)(0xE000ED34))) ;
      // Bus Fault Address Register
      _BFAR = (*((volatile unsigned long *)(0xE000ED38))) ;

    /* When the following line is hit, the variables contain the register values. */
    for( ;; );
}

/* The fault handler implementation calls a function called
prvGetRegistersFromStack(). */
void HardFault_Handler(void)
{
    __asm volatile
    (
        " tst lr, #4                                                \n"
        " ite eq                                                    \n"
        " mrseq r0, msp                                             \n"
        " mrsne r0, psp                                             \n"
        " ldr r1, [r0, #24]                                         \n"
        " ldr r2, handler2_address_const                            \n"
        " bx r2                                                     \n"
        " handler2_address_const: .word prvGetRegistersFromStack    \n"
    );
}



void MemManage_Handler(void)
{
	while(1)
	{

	}
}
void BusFault_Handler(void)
{
	while(1)
	{

	}
}
void UsageFault_Handler(void)
{
	while(1)
	{

	}
}

void DebugMon_Handler(void)
{
	while(1)
	{

	}
}


/******************************************************************************/
/*                 STM32F7xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s), for the        */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f7xx.s).                                               */
/******************************************************************************/

#if USE_PRINTF == 0
/**
  * Brief   This function handles USARTx Instance interrupt request.
  * Param   None
  * Retval  None
  */
void USARTx_IRQHandler(void)
{
  if(LL_USART_IsEnabledIT_TXE(USARTx_INSTANCE) && LL_USART_IsActiveFlag_TXE(USARTx_INSTANCE))
  {
    /* TXE flag will be automatically cleared when writing new data in TDR register */

    /* Call function in charge of handling empty DR => will lead to transmission of next character */
    USART_TXEmpty_Callback();
  }

  if(LL_USART_IsEnabledIT_TC(USARTx_INSTANCE) && LL_USART_IsActiveFlag_TC(USARTx_INSTANCE))
  {
    /* Clear TC flag */
    LL_USART_ClearFlag_TC(USARTx_INSTANCE);
    /* Call function in charge of handling end of transmission of sent character
       and prepare next charcater transmission */
    USART_CharTransmitComplete_Callback();
  }

  if(LL_USART_IsEnabledIT_ERROR(USARTx_INSTANCE) && LL_USART_IsActiveFlag_NE(USARTx_INSTANCE))
  {
    /* Call Error function */
    Error_Callback();
  }

}

#endif /* USE_PRINTF == 0 */

/**
  * @}
  */

/**
  * @}
  */
