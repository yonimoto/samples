################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tasks/userInterface.cpp 

OBJS += \
./tasks/userInterface.o 

CPP_DEPS += \
./tasks/userInterface.d 


# Each subdirectory must supply rules for building sources it contributes
tasks/%.o: ../tasks/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C++ Compiler'
	arm-elf-g++ -I"C:\Users\Public\Desktop\CmpE146Package\workspace\L2_lpc2148_freertos_cpp_spi" -I"C:\Users\Public\Desktop\CmpE146Package\workspace\L2_lpc2148_freertos_cpp_spi\FreeRTOS" -Os -Wall -Wa,-adhlns="$@.lst" -fno-exceptions -fno-rtti -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi-s -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


