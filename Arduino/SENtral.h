/**
 * @file SENtral.h
 *
 */

#ifndef _SENTRAL_h
#define _SENTRAL_h

#include <Arduino.h>

/**
 * @class SENtral
 * @brief Class from communication with the SENtral chip.
 *
 * The functionality from this class was copied from the Internet with limited documentation.
 * We have no idea what it does but it somehow works.
 */
class SENtral {
public:
	/**
	 * Read the SENtral devices. The found devices will be printed to console.
	 * This is a debug function.
	 */
	void readSENtralDevices();

	/**
	 * SENtral pass throug mode.
	 */
	void SENtralPassThroughMode();

	/**
	 * Convert a 32 bit int register to a float.
	 * @param buf the buffer containing the int.
	 * @return the float.
	 */
	float uint32_reg_to_float(uint8_t *buf);

	/**
	 * Read the SENtral data.
	 * @param destination the destination.
	 * @param deviceAddress the address.
	 */
	void readSENtralData(int16_t * destination, int deviceAddress);

	/**
	 * Read the SENtral quat data.
	 * @param destination the destination.
	 */
	void readSENtralQuatData(float * destination);

	/**
	 * Read the SENtral data.
	 * @param deviceAddress the device address.
	 * @return The read data.
	 */
	int16_t readSENtralData(int deviceAddress);

	/**
	 * Do something with new data.
	 * @param count a counter.
	 * @param address a address.
	 * @param x a x.
	 * @param y a y.
	 * @param z a z.
	 * @param factor a factor.
	 */
	void newData(int16_t count[], int address, float &x, float &y, float &z, float factor);

	/**
	 * Error.
	 */
	void error();
};

#endif //_SENTRAL_h

