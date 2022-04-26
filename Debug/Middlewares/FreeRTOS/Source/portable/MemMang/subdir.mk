################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/FreeRTOS/Source/portable/MemMang/heap_4.c 

OBJS += \
./Middlewares/FreeRTOS/Source/portable/MemMang/heap_4.o 

C_DEPS += \
./Middlewares/FreeRTOS/Source/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/FreeRTOS/Source/portable/MemMang/%.o: ../Middlewares/FreeRTOS/Source/portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32 -DSTM32F7 -DSTM32F767ZITx -DNUCLEO_F767ZI -DDEBUG -DSTM32F767xx -DUSE_HAL_DRIVER -DINCLUDE_FATFS -DUSE_RTOS_SYSTICK -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/Template/Inc" -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM7" -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/MTP/Inc" -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/MSC/Inc" -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Middlewares/FreeRTOS/Source/include" -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/CDC/Inc" -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Middlewares/STM32_USB_Host_Library/Core/Inc" -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/AUDIO/Inc" -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/HID/Inc" -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Drivers/BSP/STM32F7xx_Nucleo_144" -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Drivers/CMSIS/device" -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Drivers/HAL_Driver/Inc/Legacy" -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Drivers/CMSIS/core" -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Drivers/HAL_Driver/Inc" -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Applications/inc" -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Middlewares/FatFs/src" -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Middlewares/FatFs/src/drivers" -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Middlewares/FatFs/src/option" -I"C:/Users/bridg/Documents/DFR/DFR_CVC/Middlewares/FatFs/test" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


