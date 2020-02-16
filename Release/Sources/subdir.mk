################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/adc.c \
../Sources/assert.c \
../Sources/break.c \
../Sources/dac.c \
../Sources/led.c \
../Sources/main.c \
../Sources/pid.c \
../Sources/switch.c \
../Sources/time.c \
../Sources/uart.c 

OBJS += \
./Sources/adc.o \
./Sources/assert.o \
./Sources/break.o \
./Sources/dac.o \
./Sources/led.o \
./Sources/main.o \
./Sources/pid.o \
./Sources/switch.o \
./Sources/time.o \
./Sources/uart.o 

C_DEPS += \
./Sources/adc.d \
./Sources/assert.d \
./Sources/break.d \
./Sources/dac.d \
./Sources/led.d \
./Sources/main.d \
./Sources/pid.d \
./Sources/switch.d \
./Sources/time.d \
./Sources/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/%.o: ../Sources/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g3 -gstabs -O2 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=at90pwm3b -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


