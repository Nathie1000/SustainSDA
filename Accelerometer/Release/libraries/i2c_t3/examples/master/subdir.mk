################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
INO_SRCS += \
C:/Program\ Files\ (x86)/Arduino/hardware/teensy/avr/libraries/i2c_t3/examples/master/master.ino 

INO_DEPS += \
./libraries/i2c_t3/examples/master/master.ino.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/i2c_t3/examples/master/master.o: C:/Program\ Files\ (x86)/Arduino/hardware/teensy/avr/libraries/i2c_t3/examples/master/master.ino
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"C:/Program Files (x86)/Arduino/hardware/teensy/../tools/arm/bin/arm-none-eabi-g++" -c -Os -g -Wall -ffunction-sections -fdata-sections -nostdlib -MMD -fno-exceptions -felide-constructors -std=gnu++0x -fno-rtti -mthumb -mcpu=cortex-m4 -fsingle-precision-constant -D__MK20DX256__ -DTEENSYDUINO=129 -DARDUINO=10606 -DF_CPU=72000000 -DUSB_SERIAL -DLAYOUT_US_INTERNATIONAL   -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SPI" -I"C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\i2c_t3" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"   -Wall
	@echo 'Finished building: $<'
	@echo ' '


