################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/mem/heap_3.c 

OBJS += \
./FreeRTOS/mem/heap_3.o 

C_DEPS += \
./FreeRTOS/mem/heap_3.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/mem/%.o: ../FreeRTOS/mem/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler'
	arm-elf-gcc -I"C:\Users\Public\Desktop\CmpE146Package\workspace\L1_lpc2148_freertos_cpp_uart" -I"C:\Users\Public\Desktop\CmpE146Package\workspace\L1_lpc2148_freertos_cpp_uart\FreeRTOS" -Os -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi-s -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


