################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include Middlewares/STM32_USB_Host_Library/Core/Src/subdir.mk
-include Middlewares/STM32_USB_Host_Library/Class/Template/Src/subdir.mk
-include Middlewares/STM32_USB_Host_Library/Class/MTP/Src/subdir.mk
-include Middlewares/STM32_USB_Host_Library/Class/MSC/Src/subdir.mk
-include Middlewares/STM32_USB_Host_Library/Class/HID/Src/subdir.mk
-include Middlewares/STM32_USB_Host_Library/Class/CDC/Src/subdir.mk
-include Middlewares/STM32_USB_Host_Library/Class/AUDIO/Src/subdir.mk
-include Middlewares/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM7/subdir.mk
-include Middlewares/FreeRTOS/Source/CMSIS_RTOS/subdir.mk
-include Middlewares/FreeRTOS/Source/subdir.mk
-include Drivers/HAL_Driver/Src/Legacy/subdir.mk
-include Drivers/HAL_Driver/Src/subdir.mk
-include Drivers/BSP/STM32F7xx_Nucleo_144/subdir.mk
-include Applications/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: dfr_cvc.elf

# Tool invocations
dfr_cvc.elf: $(OBJS) $(USER_OBJS) C:\Users\f002ccx\DFR_CVC/LinkerScript.ld
	@echo 'Building target: $@'
	@echo 'Invoking: MCU GCC Linker'
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -T"C:/Users/f002ccx/DFR_CVC/LinkerScript.ld" -Wl,-Map=output.map -Wl,--gc-sections -o "dfr_cvc.elf" @"objects.list" $(USER_OBJS) $(LIBS) -lm
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

post-build:
	-@echo 'Generating binary and Printing size information:'
	arm-none-eabi-objcopy -O binary "dfr_cvc.elf" "dfr_cvc.bin"
	arm-none-eabi-size "dfr_cvc.elf"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
