################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/ssp_spi.c \
../drivers/uart0.c \
../drivers/watchdog.c 

OBJS += \
./drivers/ssp_spi.o \
./drivers/uart0.o \
./drivers/watchdog.o 

C_DEPS += \
./drivers/ssp_spi.d \
./drivers/uart0.d \
./drivers/watchdog.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/%.o: ../drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler'
	arm-elf-gcc -I"C:\Users\Public\Desktop\CmpE146Package\workspace\L2_lpc2148_freertos_cpp_spi" -I"C:\Users\Public\Desktop\CmpE146Package\workspace\L2_lpc2148_freertos_cpp_spi\FreeRTOS" -Os -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi-s -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


