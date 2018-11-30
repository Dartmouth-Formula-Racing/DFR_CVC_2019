################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_conf_template.c \
../Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c \
../Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c \
../Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c \
../Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c 

OBJS += \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_conf_template.o \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.o \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.o \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.o \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.o 

C_DEPS += \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_conf_template.d \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.d \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.d \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.d \
./Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_USB_Host_Library/Core/Src/%.o: ../Middlewares/ST/STM32_USB_Host_Library/Core/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32 -DSTM32F7 -DSTM32F767ZITx -DNUCLEO_F767ZI -DDEBUG -DSTM32F767xx -DUSE_HAL_DRIVER -DINCLUDE_FATFS -DUSE_RTOS_SYSTICK -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/ST/STM32_USB_Host_Library/Class/Template/Inc" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7" -I"C:/Users/f002bc7/workspace/dfr_cvc/Utilities/STM32F7xx_Nucleo_144" -I"C:/Users/f002bc7/workspace/dfr_cvc/inc" -I"C:/Users/f002bc7/workspace/dfr_cvc/CMSIS/device" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/ST/STM32_USB_Host_Library/Class/MTP/Inc" -I"C:/Users/f002bc7/workspace/dfr_cvc/HAL_Driver/Inc/Legacy" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/Third_Party/FreeRTOS/Source/include" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/ST/STM32_USB_Host_Library/Core/Inc" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/Third_Party/FatFs/src/drivers" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/Third_Party/FatFs/src" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/ST/STM32_USB_Host_Library/Class/AUDIO/Inc" -I"C:/Users/f002bc7/workspace/dfr_cvc/CMSIS/core" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Users/f002bc7/workspace/dfr_cvc/HAL_Driver/Inc" -I"C:/Users/f002bc7/workspace/dfr_cvc/Middlewares/ST/STM32_USB_Host_Library/Class/HID/Inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


