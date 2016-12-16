/**
 * @file BMP280.h
 *
 */

#ifndef _BMP280_h
#define _BMP280_h

#include <Arduino.h>
#include "SENtral.h"

/**
 * @class BMP280
 * @brief Class to communicate with the BMP280 chip.
 *
 * The functionality from this class was copied from the Internet with limited documentation.
 * We have no idea what it does but it somehow works.
 */
class BMP280 {
public:
	/**
	 * Initiate the BMP280.
	 */
	void BMP280Init();

	/**
	 * Read the temperature.
	 * @return The temperature in degrees Celsius.
	 */
	int32_t readBMP280Temperature();

	/**
	 * Read the pressure.
	 * @return The pressure in PA.
	 */
	int32_t readBMP280Pressure();

	/**
	 * Compensate for the temperature.
	 * @param adc_T the analog digital converter T.
	 * @return The temperature with compensation.
	 */
	int32_t BMP280_compensate_T(int32_t adc_T);

	/**
	 * Compensate for the pressure.
	 * @param adc_P the analog digital converter P.
	 * @return The pressure with compensation.
	 */
	uint32_t BMP280_compensate_P(int32_t adc_P);

	/**
	 * Do something whit new data.
	 * @param rawPressure the raw pressure.
	 * @param pressure the not raw pressure.
	 * @param rawTemp the raw temperature.
	 * @param temp the not raw temperature.
	 * @param sentral the SENtral object.
	 */
	void newData(int16_t rawPressure, float pressure, int16_t rawTemp, float temp, SENtral sentral);
};

#endif // _BMP280_h
