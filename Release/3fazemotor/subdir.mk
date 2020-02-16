################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../3fazemotor/3fazemotor.c 

OBJS += \
./3fazemotor/3fazemotor.o 

C_DEPS += \
./3fazemotor/3fazemotor.d 


# Each subdirectory must supply rules for building sources it contributes
3fazemotor/%.o: ../3fazemotor/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g3 -gstabs -O2 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=at90pwm3b -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


