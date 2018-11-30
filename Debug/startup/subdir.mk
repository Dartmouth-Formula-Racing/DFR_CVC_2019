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
	arm-none-eabi-as -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/ST/STM32_USB_Host_Library/Class/Template/Inc" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7" -I"C:/Users/f002bc7/workspace/dfr_cvc/Utilities/STM32F7xx_Nucleo_144" -I"C:/Users/f002bc7/workspace/dfr_cvc/inc" -I"C:/Users/f002bc7/workspace/dfr_cvc/CMSIS/device" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Inc" -I"C:/Users/f002bc7/workspace/dfr_cvc/HAL_Driver/Inc/Legacy" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/ST/STM32_USB_Host_Library/Core/Inc" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/Third_Party/FatFs/src" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Inc" -I"C:/Users/f002bc7/workspace/dfr_cvc/CMSIS/core" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Users/f002bc7/workspace/dfr_cvc/HAL_Driver/Inc" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


