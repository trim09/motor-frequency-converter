################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/space_vector/SVPWM.c \
../Sources/space_vector/psc.c \
../Sources/space_vector/table_sin.c \
../Sources/space_vector/vector_recalculation.c 

OBJS += \
./Sources/space_vector/SVPWM.o \
./Sources/space_vector/psc.o \
./Sources/space_vector/table_sin.o \
./Sources/space_vector/vector_recalculation.o 

C_DEPS += \
./Sources/space_vector/SVPWM.d \
./Sources/space_vector/psc.d \
./Sources/space_vector/table_sin.d \
./Sources/space_vector/vector_recalculation.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/space_vector/%.o: ../Sources/space_vector/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g3 -gstabs -O2 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=at90pwm3b -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


