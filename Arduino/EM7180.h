/**
 * @file EM7180.h
 *
 */

#ifndef _EM7180_h
#define _EM7180_h

#include <Arduino.h>

/**
 * @class EM7180
 * @brief Class from communication with the EM7180 chip.
 *
 * The functionality from this class was copied from the Internet with limited documentation.
 * We have no idea what it does but it somehow works.
 */
class EM7180 {
public:
	/**
	 * Detect the available sensors. The detected sensor will be printed to console.
	 * This is a debug function.
	 */
	void SensorsDetected();

	/**
	 * Initiate the EM7180.
	 */
	void EM7180InitState();

	/**
	 * Setup the sensors.
	 */
	void setupSensors();

	/**
	 * Configure the OM.
	 */
	void configureOM();

	/**
	 * Read the sensor FS.
	 * @param EM7180FS
	 * @param address the sensor address.
	 * @param paramIndex the parameter index.
	 */
	void readSensorFS(uint16_t EM7180FS, int address, int paramIndex);

	/**
	 * Set the module accuracy FS.
	 * @param moduleFS the module.
	 * @param accelerometerFS the accelerometer FS.
	 * @param address the sensor address.
	 */
	void EM7180SetModuleAccFS(uint16_t moduleFS, uint16_t accelerometerFS, int address);

	/**
	 * Set the initialize parameters.
	 * @param param the parameter.
	 * @param paramValue the parameter value.
	 */
	void EM7180SetIntInParam(uint8_t param, uint32_t paramValue);

	/**
	 * Detect the EM7180 status. The status will be printed to console.
	 * This is a debug function.
	 */
	void EM7180Status();

	/**
	 * Detect the state of the sensors. The detected sate will be printed to console.
	 * This is a debug function.
	 */
	void sensorState();


	//bool setDLPFMode(const uint8_t mode);
};
#endif

