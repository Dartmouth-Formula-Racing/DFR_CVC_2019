/*
 * cvc_spi.h
 *
 *  Created on: Jan 9, 2019
 *      Author: f002ccx
 */

#ifndef INC_CVC_SPI_H_
#define INC_CVC_SPI_H_


/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_rcc.h"
#include "stm32f7xx_ll_system.h"
#include "stm32f7xx_ll_utils.h"
#include "stm32f7xx_ll_gpio.h"
#include "stm32f7xx_ll_exti.h"
#include "stm32f7xx_ll_spi.h"
#include "stm32f7xx_ll_pwr.h"

#include "stm32f7xx_it.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "cvc_config.h"

/* Defines -------------------------------------------------------------------*/

/* Definition of PLC clk configs*/

#define SPI1_CLK_ENABLE()			LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1)

#if PROTOTYPE == 1
#define SPI1_GPIO_CLK_ENABLE()		LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);\
									LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);\
									LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG)
#else

#define SPI1_GPIO_CLK_ENABLE()		LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);\
									LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);\
									LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG);\
									LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB)

#endif /*CVC_PROTOTYPE == 0 */

/* Definition of PLC SPI pins */
/* SCK Pin */
#define PLC_SCK_PIN				LL_GPIO_PIN_5
#define PLC_SCK_GPIO_PORT		GPIOA
#define PLC_SCK_AF				LL_GPIO_AF_5

/* MISO pin */
#define PLC_MISO_PIN			LL_GPIO_PIN_6
#define PLC_MISO_GPIO_PORT		GPIOA
#define PLC_MISO_AF				LL_GPIO_AF_5

/* MOSI pin */
#if CVC_PROTOTYPE == 1

#define PLC_MOSI_PIN			LL_GPIO_PIN_7	// Pin 5 for PCB
#define PLC_MOSI_GPIO_PORT		GPIOA			// GPIOB for PCB

#else

#define PLC_MOSI_PIN			LL_GPIO_PIN_5
#define PLC_MOSI_GPIO_PORT		GPIOB

#endif /* CVC_PROTOTYPE == 1 */

#define PLC_MOSI_AF				LL_GPIO_AF_5

/* CS1 (for CLT01-38SQ7) */
#define PLC_CS1_PIN				LL_GPIO_PIN_15
#define PLC_CS1_GPIO_PORT		GPIOD

/* CS2 (for VNI8200XP) */
#define PLC_CS2_PIN				LL_GPIO_PIN_14
#define	PLC_CS2_GPIO_PORT		GPIOD

/* OUTEN (for VNI8200XP) */
#define PLC_OUTEN_PIN			LL_GPIO_PIN_12
#define PLC_OUTEN_GPIO_PORT		GPIOG


/* Definitions for Queues */
#define PLC_TRANSMIT_QUEUE_LENGTH		100U

/* Typedefs ----------------------------------------------------------*/

volatile typedef struct SPI_inputs_vector_s
{
	uint8_t ICE_enable;						// PLC input 1
	uint8_t Motor_enable;					// PLC input 2
	uint8_t Ready_to_drive;					// PLC input 3
	uint8_t Dash_BRB_press;					// PLC input 4
	uint8_t	IMD_safety_circuit_fault;		// PLC input 5
	uint8_t BMS_safety_circuit_fault;		// PLC input 6
	uint8_t Bamocar_safety_circuit_fault;	// PLC input 7
	//extra input							// PLC input 8

} SPI_inputs_vector_t;

volatile typedef struct SPI_outputs_vector_s
{
	uint8_t	safety;					// PLC output 1
	uint8_t	ready_to_drive;			// PLC output 2
	uint8_t	rfg;					// PLC output 3
	uint8_t ignition_kill;			// PLC output 4
	uint8_t	downshift_solenoid;		// PLC output 5
	uint8_t upshift_solenoid;		// PLC output 6
	//extra output					// PLC output 7
	//extra output					// PLC output 8

} SPI_outputs_vector_t;


typedef struct CLT_Read_s
{
	int	High	:	1;	//This is bit 0
	int	Low		:	1;	//This is bit 1
	int	PC4		:	1;	//This is bit 2
	int PC3		: 	1;	//This is bit 3

	int PC2		:	1;	//This is bit 4
	int PC1		:	1;	//This is bit 5
	int OTA		:	1;	//This is bit 6
	int UVA		:	1;	//This is bit 7

	int IN1		:	1;	//This is bit 8
	int IN2		:	1;	//This is bit 9
	int IN3		:	1;	//This is bit 10
	int IN4		:	1;	//This is bit 11
	int IN5		:	1;	//This is bit 12
	int IN6		:	1;	//This is bit 13
	int IN7		:	1;	//This is bit 14
	int IN8		:	1;	//This is bit 15

}	CLT_Read_t;

typedef struct VNI_Read_s
{
	uint16_t	nP0		:	1;	//This is bit 0
	uint16_t	P0		:	1;	//This is bit 1
	uint16_t	P1		:	1;	//This is bit 2
	uint16_t	P2		:	1;	//This is bit 3

	uint16_t	PG		:	1;	//This is bit 4
	uint16_t	PC		:	1;	//This is bit 5
	uint16_t	TWARN	:	1;	//This is bit 6
	uint16_t	FBOK	:	1;	//This is bit 7

	uint16_t	F1		:	1;	//This is bit 8
	uint16_t	F2		:	1;	//This is bit 9
	uint16_t	F3		:	1;	//This is bit 10
	uint16_t	F4		:	1;	//This is bit 11
	uint16_t	F5		:	1;	//This is bit 12
	uint16_t	F6		:	1;	//This is bit 13
	uint16_t	F7		:	1;	//This is bit 14
	uint16_t	F8		:	1;	//This is bit 15

} 	VNI_Read_t;

typedef struct	VNI_Write_s
{
	uint16_t	nP0		:	1;	//This is bit 0
	uint16_t	P0		:	1;	//This is bit 1
	uint16_t	P1		:	1;	//This is bit 2
	uint16_t	P2		:	1;	//This is bit 3

	uint16_t	SPARE	:	4;	//This is bits 4-7

	uint16_t	IN1		:	1;	//This is bit 8
	uint16_t	IN2		:	1;	//This is bit 9
	uint16_t	IN3		:	1;	//This is bit 10
	uint16_t	IN4		:	1;	//This is bit 11
	uint16_t	IN5		:	1;	//This is bit 12
	uint16_t	IN6		:	1;	//This is bit 13
	uint16_t	IN7		:	1;	//This is bit 14
	uint16_t	IN8		:	1;	//This is bit 15

} 	VNI_Write_t;


typedef union CLT_Read_u
{
	uint16_t	word;
	uint8_t		bytes[2];
	CLT_Read_t	bit;
} CLT_Read_u_t;

typedef union VNI_Read_u
{
	uint16_t	word;
	uint8_t		bytes[2];
	VNI_Read_t	bit;
} VNI_Read_u_t;

typedef union VNI_Write_u
{
	uint16_t	word;
	uint8_t		bytes[2];
	VNI_Write_t	bit;
} VNI_Write_u_t;

typedef enum SPI_io_states_t
{
	wait_for_CLT,
	wait_for_VNI,
	wait_for_next_transmission

} SPI_io_states_t;


/* External Variables ------------------------------------------------------------*/
extern volatile	CLT_Read_u_t CLT_Read;
extern volatile uint16_t	CLT_Write;	//random 16 bits to initiate transfer with CLT
extern volatile VNI_Read_u_t VNI_Read;

extern volatile SPI_inputs_vector_t		SPI_inputs_vector;
extern volatile SPI_outputs_vector_t	SPI_outputs_vector;

extern volatile SPI_io_states_t SPI_io_state;

extern volatile SemaphoreHandle_t SPI_Inputs_Vector_Mutex;
extern volatile SemaphoreHandle_t SPI_Outputs_Vector_Mutex;

/* Function prototypes -----------------------------------------------*/
void PLC_Routine_Task(void * parameters);
void PLC_routine_ISR_callback(void);

void Configure_SPI(void);
void Activate_SPI(void);

void initiate_SPI_transmission(void);
void SPI_routine(void);

void add_to_SPI_input_buffer(uint16_t new_data);

void SPI_PLC_Set_Inputs(void);
void SPI_PLC_Set_Outputs(void);
void set_SPI_parity_check_bit_outputs(volatile VNI_Write_u_t *WriteValue);

CLT_Read_u_t debounce_SPI_input(void);


#endif /* INC_CVC_SPI_H_ */
