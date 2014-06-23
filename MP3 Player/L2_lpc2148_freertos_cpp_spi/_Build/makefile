################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include utils/src/subdir.mk
-include tasks/subdir.mk
-include fat/subdir.mk
-include fat/option/subdir.mk
-include drivers/subdir.mk
-include System/subdir.mk
-include System/startup/subdir.mk
-include FreeRTOS/src/subdir.mk
-include FreeRTOS/mem/subdir.mk
-include FreeRTOS/hooks/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
L2_lpc2148_freertos_cpp_spi.hex \

SECONDARY_LIST += \
L2_lpc2148_freertos_cpp_spi.lst \

SECONDARY_SIZE += \
L2_lpc2148_freertos_cpp_spi.siz \


# All Target
all: L2_lpc2148_freertos_cpp_spi.elf secondary-outputs

# Tool invocations
L2_lpc2148_freertos_cpp_spi.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Windows GCC C++ Linker'
	arm-elf-g++ -T"..\System\startup\FREERTOS_LPC2148.ld" -nostartfiles -Wl,-Map,L2_lpc2148_freertos_cpp_spi.map -mcpu=arm7tdmi-s -o"L2_lpc2148_freertos_cpp_spi.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

L2_lpc2148_freertos_cpp_spi.hex: L2_lpc2148_freertos_cpp_spi.elf
	@echo 'Invoking: ARM Windows GNU Create Flash Image'
	arm-elf-objcopy -O ihex L2_lpc2148_freertos_cpp_spi.elf "L2_lpc2148_freertos_cpp_spi.hex"
	@echo 'Finished building: $@'
	@echo ' '

L2_lpc2148_freertos_cpp_spi.lst: L2_lpc2148_freertos_cpp_spi.elf
	@echo 'Invoking: ARM Windows GNU Create Listing'
	arm-elf-objdump -h -S L2_lpc2148_freertos_cpp_spi.elf >"L2_lpc2148_freertos_cpp_spi.lst"
	@echo 'Finished building: $@'
	@echo ' '

L2_lpc2148_freertos_cpp_spi.siz: L2_lpc2148_freertos_cpp_spi.elf
	@echo 'Invoking: ARM Windows GNU Print Size'
	arm-elf-size  --format=berkeley -t L2_lpc2148_freertos_cpp_spi.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(SECONDARY_FLASH)$(CXX_DEPS)$(S_UPPER_DEPS)$(SECONDARY_LIST)$(C++_DEPS)$(SECONDARY_SIZE)$(ASM_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS) L2_lpc2148_freertos_cpp_spi.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
