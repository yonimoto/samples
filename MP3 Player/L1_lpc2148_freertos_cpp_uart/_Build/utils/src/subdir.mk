################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../utils/src/timer.cpp 

C_SRCS += \
../utils/src/rprintf.c 

OBJS += \
./utils/src/rprintf.o \
./utils/src/timer.o 

C_DEPS += \
./utils/src/rprintf.d 

CPP_DEPS += \
./utils/src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
utils/src/%.o: ../utils/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler'
	arm-elf-gcc -I"C:\Users\Public\Desktop\CmpE146Package\workspace\L1_lpc2148_freertos_cpp_uart" -I"C:\Users\Public\Desktop\CmpE146Package\workspace\L1_lpc2148_freertos_cpp_uart\FreeRTOS" -Os -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi-s -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

utils/src/%.o: ../utils/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C++ Compiler'
	arm-elf-g++ -I"C:\Users\Public\Desktop\CmpE146Package\workspace\L1_lpc2148_freertos_cpp_uart" -I"C:\Users\Public\Desktop\CmpE146Package\workspace\L1_lpc2148_freertos_cpp_uart\FreeRTOS" -Os -Wall -Wa,-adhlns="$@.lst" -fno-exceptions -fno-rtti -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi-s -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


