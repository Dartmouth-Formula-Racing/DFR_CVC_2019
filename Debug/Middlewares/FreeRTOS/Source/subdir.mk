################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/FreeRTOS/Source/croutine.c \
../Middlewares/FreeRTOS/Source/event_groups.c \
../Middlewares/FreeRTOS/Source/list.c \
../Middlewares/FreeRTOS/Source/queue.c \
../Middlewares/FreeRTOS/Source/tasks.c \
../Middlewares/FreeRTOS/Source/timers.c 

OBJS += \
./Middlewares/FreeRTOS/Source/croutine.o \
./Middlewares/FreeRTOS/Source/event_groups.o \
./Middlewares/FreeRTOS/Source/list.o \
./Middlewares/FreeRTOS/Source/queue.o \
./Middlewares/FreeRTOS/Source/tasks.o \
./Middlewares/FreeRTOS/Source/timers.o 

C_DEPS += \
./Middlewares/FreeRTOS/Source/croutine.d \
./Middlewares/FreeRTOS/Source/event_groups.d \
./Middlewares/FreeRTOS/Source/list.d \
./Middlewares/FreeRTOS/Source/queue.d \
./Middlewares/FreeRTOS/Source/tasks.d \
./Middlewares/FreeRTOS/Source/timers.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/FreeRTOS/Source/%.o: ../Middlewares/FreeRTOS/Source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32 -DSTM32F7 -DSTM32F767ZITx -DNUCLEO_F767ZI -DDEBUG -DSTM32F767xx -DUSE_HAL_DRIVER -DINCLUDE_FATFS -DUSE_RTOS_SYSTICK -I"C:/Users/f002ccx/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/Template/Inc" -I"C:/Users/f002ccx/DFR_CVC/Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM7" -I"C:/Users/f002ccx/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/MTP/Inc" -I"C:/Users/f002ccx/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/MSC/Inc" -I"C:/Users/f002ccx/DFR_CVC/Middlewares/FreeRTOS/Source/include" -I"C:/Users/f002ccx/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/CDC/Inc" -I"C:/Users/f002ccx/DFR_CVC/Middlewares/STM32_USB_Host_Library/Core/Inc" -I"C:/Users/f002ccx/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/AUDIO/Inc" -I"C:/Users/f002ccx/DFR_CVC/Middlewares/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Users/f002ccx/DFR_CVC/Middlewares/STM32_USB_Host_Library/Class/HID/Inc" -I"C:/Users/f002ccx/DFR_CVC/Drivers/BSP/STM32F7xx_Nucleo_144" -I"C:/Users/f002ccx/DFR_CVC/Drivers/CMSIS/device" -I"C:/Users/f002ccx/DFR_CVC/Drivers/HAL_Driver/Inc/Legacy" -I"C:/Users/f002ccx/DFR_CVC/Drivers/CMSIS/core" -I"C:/Users/f002ccx/DFR_CVC/Drivers/HAL_Driver/Inc" -I"C:/Users/f002ccx/DFR_CVC/Applications/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


