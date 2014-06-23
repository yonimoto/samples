################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../System/cppmem.cpp 

C_SRCS += \
../System/cpu.c \
../System/syscalls.c 

OBJS += \
./System/cppmem.o \
./System/cpu.o \
./System/syscalls.o 

C_DEPS += \
./System/cpu.d \
./System/syscalls.d 

CPP_DEPS += \
./System/cppmem.d 


# Each subdirectory must supply rules for building sources it contributes
System/%.o: ../System/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C++ Compiler'
	arm-elf-g++ -I"C:\Users\Public\Desktop\CmpE146Package\workspace\L1_lpc2148_freertos_cpp_uart" -I"C:\Users\Public\Desktop\CmpE146Package\workspace\L1_lpc2148_freertos_cpp_uart\FreeRTOS" -Os -Wall -Wa,-adhlns="$@.lst" -fno-exceptions -fno-rtti -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi-s -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

System/%.o: ../System/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler'
	arm-elf-gcc -I"C:\Users\Public\Desktop\CmpE146Package\workspace\L1_lpc2148_freertos_cpp_uart" -I"C:\Users\Public\Desktop\CmpE146Package\workspace\L1_lpc2148_freertos_cpp_uart\FreeRTOS" -Os -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi-s -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


