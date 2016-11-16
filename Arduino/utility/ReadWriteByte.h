// ReadWriteByte.h

#ifndef _READWRITEBYTE_h
#define _READWRITEBYTE_h

#include <Arduino.h>

class ReadWriteByte {
public:
	static void writeByte(uint8_t address, uint8_t subAddress, uint8_t data);
	static bool writeRegister(const uint8_t register_addr, const uint8_t mpuDefaultRegister, const uint8_t value);
	static bool writeMaskedRegister(const uint8_t register_addr, const uint8_t mpuDefaultRegister, const uint8_t mask, const uint8_t value);
	static uint8_t readRegister(const uint8_t register_addr, const uint8_t mpuDefaultRegister);
	static void M24512DFMreadBytes(uint8_t device_address, uint8_t data_address1, uint8_t data_address2, uint8_t count, uint8_t * dest);
	static uint8_t readByte(uint8_t address, uint8_t subAddress);
	static void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest);
	static bool I2Cscan();
};

#endif // _READWRITEBYTE_h
