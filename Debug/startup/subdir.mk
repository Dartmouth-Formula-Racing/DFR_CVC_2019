################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f767xx.s 

OBJS += \
./startup/startup_stm32f767xx.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -I"C:/Users/f002ccx/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/Template/Inc" -I"C:/Users/f002ccx/DFR_CVC/Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM7" -I"C:/Users/f002ccx/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/MTP/Inc" -I"C:/Users/f002ccx/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/MSC/Inc" -I"C:/Users/f002ccx/DFR_CVC/Middlewares/FreeRTOS/Source/include" -I"C:/Users/f002ccx/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/CDC/Inc" -I"C:/Users/f002ccx/DFR_CVC/Middlewares/STM32_USB_Host_Library/Core/Inc" -I"C:/Users/f002ccx/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/AUDIO/Inc" -I"C:/Users/f002ccx/DFR_CVC/Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Users/f002ccx/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/HID/Inc" -I"C:/Users/f002ccx/DFR_CVC/Drivers/BSP/STM32F7xx_Nucleo_144" -I"C:/Users/f002ccx/DFR_CVC/Drivers/HAL_Driver/Inc/Legacy" -I"C:/Users/f002ccx/DFR_CVC/Drivers/HAL_Driver/Inc" -I"C:/Users/f002ccx/DFR_CVC/Drivers/CMSIS/device" -I"C:/Users/f002ccx/DFR_CVC/Drivers/CMSIS/core" -I"C:/Users/f002ccx/DFR_CVC/Applications/inc" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


