################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Sources/subdir.mk
-include Sources/space_vector/subdir.mk
-include Sources/control/subdir.mk
-include 3fazemotor/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
3fazemotor.lss \

FLASH_IMAGE += \
3fazemotor.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: 3fazemotor.elf secondary-outputs

# Tool invocations
3fazemotor.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,3fazemotor.map -mmcu=at90pwm3b -o"3fazemotor.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

3fazemotor.lss: 3fazemotor.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S 3fazemotor.elf  >"3fazemotor.lss"
	@echo 'Finished building: $@'
	@echo ' '

3fazemotor.hex: 3fazemotor.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex 3fazemotor.elf  "3fazemotor.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: 3fazemotor.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=at90pwm3b 3fazemotor.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) 3fazemotor.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
