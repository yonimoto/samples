################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../MP3/button.cpp \
../MP3/control.cpp \
../MP3/mp3.cpp 

OBJS += \
./MP3/button.o \
./MP3/control.o \
./MP3/mp3.o 

CPP_DEPS += \
./MP3/button.d \
./MP3/control.d \
./MP3/mp3.d 


# Each subdirectory must supply rules for building sources it contributes
MP3/%.o: ../MP3/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Windows GCC C++ Compiler'
	arm-elf-g++ -I"C:\Users\Public\Desktop\CmpE146Package\workspace\L4_lpc2148_freertos_cpp_mp3" -I"C:\Users\Public\Desktop\CmpE146Package\workspace\L4_lpc2148_freertos_cpp_mp3\FreeRTOS" -Os -Wall -Wa,-adhlns="$@.lst" -fno-exceptions -fno-rtti -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi-s -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


