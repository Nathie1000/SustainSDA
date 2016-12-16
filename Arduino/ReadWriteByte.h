/**
 * @file ReadWriteByte.h
 *
 */

#ifndef _READWRITEBYTE_h
#define _READWRITEBYTE_h

#include <Arduino.h>

/**
 * @class ReadWriteByte
 * @brief Class containing some static function to read/write bytes using a I2C protocol.
 */
class ReadWriteByte {
public:
	/**
	 * Write byte to address.
	 * @param address the address.
	 * @param subAddress an optional sub address.
	 * @param data the data to write.
	 */
	static void writeByte(uint8_t address, uint8_t subAddress, uint8_t data);

	/**
	 * Write byte to a register.
	 * @param register_addr the register address.
	 * @param mpuDefaultRegister the default register.
	 * @param value the value to write.
	 * @return return True on success or false on failure.
	 */
	static bool writeRegister(const uint8_t register_addr, const uint8_t mpuDefaultRegister, const uint8_t value);

	/**
	 * Write byte to a register with a mask.
	 * @param register_addr the register address.
	 * @param mpuDefaultRegister the default register.
	 * @param mask the mask.
	 * @param value the value to write.
	 * @return True on success or false on failure.
	 */
	static bool writeMaskedRegister(const uint8_t register_addr, const uint8_t mpuDefaultRegister, const uint8_t mask, const uint8_t value);

	/**
	 * Read from a register.
	 * @param register_addr the register address.
	 * @param mpuDefaultRegister the default register.
	 * @return The read value.
	 */
	static uint8_t readRegister(const uint8_t register_addr, const uint8_t mpuDefaultRegister);

	/**
	 * Special M24512DF read multiple bytes.
	 * @param device_address the device address.
	 * @param data_address1 the first data address.
	 * @param data_address2 the second data address.
	 * @param count the amount of bytes to read.
	 * @param dest the buffer to store the bytes in.
	 */
	static void M24512DFMreadBytes(uint8_t device_address, uint8_t data_address1, uint8_t data_address2, uint8_t count, uint8_t * dest);

	/**
	 * Read a single byte using the I2C protocol.
	 * @param address the address to read from.
	 * @param subAddress an optional sub address.
	 * @return the read byte.
	 */
	static uint8_t readByte(uint8_t address, uint8_t subAddress);

	/**
	 * Read multiple bytes using the I2C protocol.
	 * @param address the address to read from.
	 * @param subAddress an optional sub address.
	 * @param count the amount of bytes to read.
	 * @param dest the buffer to store the bytes in.
	 */
	static void readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest);

	/**
	 * Scan for any I2C devices on the bus.
	 * @return True if any I2C device is found, fasle if none are found.
	 */
	static bool I2Cscan();
};

#endif // _READWRITEBYTE_h
