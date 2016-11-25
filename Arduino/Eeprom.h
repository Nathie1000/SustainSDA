/**
 * @file Eeprom.h
 *
 */

#ifndef _EEPROM_h
#define _EEPROM_h

#include <Arduino.h>

/**
 * @class Eeprom
 * @brief Class to read the motion sensor EEPROM.
 *
 * Do not confuse this with the Arduino EEPROM class.
 * The functionality from this call was copied from the Internet with limited documentation.
 * We have no idea what it does but it somehow works.
 */
class Eeprom {
private:
	uint8_t mMode = 0x02;        // 2 for 8 hz, 6 for 100 hz continuous magnetometer data read
	// Specify sensor full scale
	float magCalibration[3] = { 0, 0, 0 };  // factory mag calibration and mag bias
	float q[4] = { 1.0f, 0.0f, 0.0f, 0.0f };    // vector to hold quaternion
	float aRes =0.0f, gRes = 0.0f, mRes = 0.0f;

public:

	/**
	 * Initiate EEPROM.
	 * @return true if initiation was successful, false if not.
	 */
	bool initEEPROM();

	/**
	 * Initiate MPU9250.
	 */
	void initMPU9250();

	/**
	 * Initiate AK8963.
	 * @param destination
	 */
	void initAK8963(float * destination);

	/**
	 * This does something?
	 * @param dest1 the first destination.
	 * @param dest2 the second destination.
	 */
	void magcalMPU9250(float * dest1, float * dest2);

	/**
	 * Get a average value of raw data.
	 * @param rawData the raw data input.
	 * @param deviceAverage device average output.
	 * @param deviceAverage2 another device average output.
	 */
	void getAverageValue(uint8_t rawData[], int16_t deviceAverage[], int16_t deviceAverage2[]);

	/**
	 * Start the MPU9250 self test.
	 * @param destination
	 */
	void MPU9250SelfTest(float * destination);

	/**
	 * Get the magnetometer results.
	 */
	void getMagResult(); 

	/**
	 * Get the gyroscope results.
	 */
	void getGyroResult(); 

	/**
	 * Get the accelerometer results.
	 */
	void getAccelResult();

	/**
	 * This does something?
	 * @param dest1 the first destination.
	 * @param dest2 the second destination.
	 */
	void accelGyroCalMPU9250(float * dest1, float * dest2);

	/**
	 * Read the mag data.
	 * @param destination the destination.
	 */
	void readMagData(int16_t * destination);

	/**
	 * Read some other data.
	 * @param destination the destination.
	 * @param deviceAddress the device address.
	 */
	void readData(int16_t * destination, int deviceAddress);

	/**
	 * This updates something.
	 * @param accelX the accelerometer x-axis.
	 * @param accelY the accelerometer y-axis.
	 * @param accelZ the accelerometer z-axis.
	 * @param gyroX the gyroscope x-axis.
	 * @param gyroY the gyroscope y-axis.
	 * @param gyroZ the gyroscope z-axis.
	 * @param magX the magnetometer x-axis.
	 * @param magY the magnetometer y-axis.
	 * @param magZ the magnetometer-axis.
	 * @param deltat the time interval.
	 */
	void MadgwickQuaternionUpdate(float accelX, float accelY, float accelZ, float gyroX, float gyroY, float gyroZ, float magX, float magY, float magZ, float deltat);

	/**
	 * Calculate the yawn, pitch and roll from the rotation.
	 * @param rotation the rotation.
	 * @return The yawn, pitch and roll stuffed in a float somehow.
	 */
	float calculateSoftwareYPR(char rotation);

	/**
	 * Calculate the yawn, pitch and roll from the rotation.
	 * @param rotation the rotation.
	 * @param Quat i have no clue what this is.
	 * @return The yawn, pitch and roll stuffed in a float somehow.
	 */
	float calculateHardwareYPR(char rotation, float Quat[]);
};

#endif // _EEPROM_h
