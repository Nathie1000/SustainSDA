################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -f

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include libraries/i2c_t3/examples/timeout/subdir.mk
-include libraries/i2c_t3/examples/slave_range/subdir.mk
-include libraries/i2c_t3/examples/slave/subdir.mk
-include libraries/i2c_t3/examples/scanner/subdir.mk
-include libraries/i2c_t3/examples/quad_master/subdir.mk
-include libraries/i2c_t3/examples/multiplexed_master/subdir.mk
-include libraries/i2c_t3/examples/master/subdir.mk
-include libraries/i2c_t3/examples/interrupt/subdir.mk
-include libraries/i2c_t3/examples/dual_bus_master_slave/subdir.mk
-include libraries/i2c_t3/subdir.mk
-include libraries/SPI/examples/DigitalPotControl/subdir.mk
-include libraries/SPI/examples/BarometricPressureSensor/subdir.mk
-include libraries/SPI/subdir.mk
-include core/core/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
AR += \
arduino.ar \

ELF += \
Accelerometer.elf \

EEP += \
Accelerometer.eep \

FLASH_IMAGE += \
Accelerometer.hex \


# All Target
all: Accelerometer

# Tool invocations
Accelerometer: $(FLASH_IMAGE)
	@echo 'Building target: $@'
	@echo 'Printing size:'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/arm/bin/arm-none-eabi-size" -A "C:/Users/Bianca/EclipseMinorDAS/Accelerometer/Release/Accelerometer.elf"
	@echo 'Finished building target: $@'
	@echo ' '

arduino.ar: $(AR_OBJ)
	@echo 'Starting archiver'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/arm/bin/arm-none-eabi-ar" rcs    arduino.ar $(AR_OBJ)
	@echo 'Finished building: $@'
	@echo ' '

Accelerometer.elf: $(LINK_OBJ) $(AR)
	@echo 'Starting combiner'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/arm/bin/arm-none-eabi-gcc" -Os -Wl,--gc-sections,--relax,--defsym=__rtc_localtime=1474550339 "-TC:/Program Files (x86)/Arduino/hardware/teensy/avr/cores/teensy3/mk20dx256.ld" --specs=nano.specs -mthumb -mcpu=cortex-m4 -fsingle-precision-constant -o "C:/Users/Bianca/EclipseMinorDAS/Accelerometer/Release/Accelerometer.elf"   $(LINK_OBJ) $(AR)    "-LC:/Users/Bianca/EclipseMinorDAS/Accelerometer/Release" -larm_cortexM4l_math -lm
	@echo 'Finished building: $@'
	@echo ' '

Accelerometer.eep: $(ELF)
	@echo 'Create eeprom image'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/arm/bin/arm-none-eabi-objcopy" -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 "C:/Users/Bianca/EclipseMinorDAS/Accelerometer/Release/Accelerometer.elf" "C:/Users/Bianca/EclipseMinorDAS/Accelerometer/Release/Accelerometer.eep"
	@echo 'Finished building: $@'
	@echo ' '

Accelerometer.hex: $(EEP)
	@echo 'Create Flash image (ihex format)'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/arm/bin/arm-none-eabi-objcopy" -O ihex -R .eeprom "C:/Users/Bianca/EclipseMinorDAS/Accelerometer/Release/Accelerometer.elf" "C:/Users/Bianca/EclipseMinorDAS/Accelerometer/Release/Accelerometer.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_UPPER_DEPS)$(PDE_DEPS)$(C_DEPS)$(AR)$(EEP)$(FLASH_IMAGE)$(CC_DEPS)$(AR_OBJ)$(C++_DEPS)$(LINK_OBJ)$(CXX_DEPS)$(ASM_DEPS)$(INO_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(ELF)$(CPP_DEPS) Accelerometer
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
