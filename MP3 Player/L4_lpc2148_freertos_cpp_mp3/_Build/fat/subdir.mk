################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../fat/diskio.c \
../fat/ff.c \
../fat/rtc.c 

OBJS += \
./fat/diskio.o \
./fat/ff.o \
./fat/rtc.o 

C_DEPS += \
./fat/diskio.d \
./fat/ff.d \
./fat/rtc.d 


# Each subdirectory must supply rules for building sources it contributes
fat/%.o: ../fat/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C Compiler'
	arm-elf-gcc -I"C:\Users\Public\Desktop\CmpE146Package\workspace\L4_lpc2148_freertos_cpp_mp3" -I"C:\Users\Public\Desktop\CmpE146Package\workspace\L4_lpc2148_freertos_cpp_mp3\FreeRTOS" -Os -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi-s -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


