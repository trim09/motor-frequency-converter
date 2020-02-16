################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/control/controlVF.c \
../Sources/control/current_limitation.c \
../Sources/control/mc_control.c \
../Sources/control/speed.c \
../Sources/control/speed_control.c \
../Sources/control/timer0.c 

OBJS += \
./Sources/control/controlVF.o \
./Sources/control/current_limitation.o \
./Sources/control/mc_control.o \
./Sources/control/speed.o \
./Sources/control/speed_control.o \
./Sources/control/timer0.o 

C_DEPS += \
./Sources/control/controlVF.d \
./Sources/control/current_limitation.d \
./Sources/control/mc_control.d \
./Sources/control/speed.d \
./Sources/control/speed_control.d \
./Sources/control/timer0.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/control/%.o: ../Sources/control/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g3 -gstabs -O2 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=at90pwm3b -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


