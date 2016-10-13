// ReadWriteByte.h

#ifndef _READWRITEBYTE_h
#define _READWRITEBYTE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
 
#include <i2c_t3.h>

void writeByte(uint8_t address, uint8_t subAddress, uint8_t data);
void M24512DFMreadBytes(uint8_t device_address, uint8_t data_address1, uint8_t data_address2, uint8_t count, uint8_t * dest);
uint8_t readByte(uint8_t address, uint8_t subAddress);
void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest);
void I2Cscan();


#endif

