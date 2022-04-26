################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/HAL_Driver/Src/Legacy/stm32f7xx_hal_can.c 

OBJS += \
./Drivers/HAL_Driver/Src/Legacy/stm32f7xx_hal_can.o 

C_DEPS += \
./Drivers/HAL_Driver/Src/Legacy/stm32f7xx_hal_can.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/HAL_Driver/Src/Legacy/%.o: ../Drivers/HAL_Driver/Src/Legacy/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32 -DSTM32F7 -DSTM32F767ZITx -DNUCLEO_F767ZI -DDEBUG -DSTM32F767xx -DUSE_HAL_DRIVER -DINCLUDE_FATFS -DUSE_RTOS_SYSTICK -I"O:/DFR/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/Template/Inc" -I"O:/DFR/DFR_CVC/Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM7" -I"O:/DFR/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/MTP/Inc" -I"O:/DFR/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/MSC/Inc" -I"O:/DFR/DFR_CVC/Middlewares/FreeRTOS/Source/include" -I"O:/DFR/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/CDC/Inc" -I"O:/DFR/DFR_CVC/Middlewares/STM32_USB_Host_Library/Core/Inc" -I"O:/DFR/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/AUDIO/Inc" -I"O:/DFR/DFR_CVC/Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"O:/DFR/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/HID/Inc" -I"O:/DFR/DFR_CVC/Drivers/BSP/STM32F7xx_Nucleo_144" -I"O:/DFR/DFR_CVC/Drivers/CMSIS/device" -I"O:/DFR/DFR_CVC/Drivers/HAL_Driver/Inc/Legacy" -I"O:/DFR/DFR_CVC/Drivers/CMSIS/core" -I"O:/DFR/DFR_CVC/Drivers/HAL_Driver/Inc" -I"O:/DFR/DFR_CVC/Applications/inc" -I"O:/DFR/DFR_CVC/Middlewares/FatFs/src" -I"O:/DFR/DFR_CVC/Middlewares/FatFs/src/drivers" -I"O:/DFR/DFR_CVC/Middlewares/FatFs/src/option" -I"O:/DFR/DFR_CVC/Middlewares/FatFs/test" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


