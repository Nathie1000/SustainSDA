/*
 * BMP280.cpp
 *
 *  Created on: Sep 19, 2016
 *      Author: Bianca
 */

//Barometer and thermometer

#include "BMP280.h"

enum Posr {
	P_OSR_00 = 0,  // no op
	P_OSR_01,
	P_OSR_02,
	P_OSR_04,
	P_OSR_08,
	P_OSR_16
};
enum Tosr {
	T_OSR_00 = 0,  // no op
	T_OSR_01,
	T_OSR_02,
	T_OSR_04,
	T_OSR_08,
	T_OSR_16
};
enum IIRFilter {
	full = 0,  // bandwidth at full sample rate
	BW0_223ODR,
	BW0_092ODR,
	BW0_042ODR,
	BW0_021ODR // bandwidth at 0.021 x sample rate
};
enum Mode {
	BMP280Sleep = 0, forced, forced2, normal
};
enum SBy {
	t_00_5ms = 0,
	t_62_5ms,
	t_125ms,
	t_250ms,
	t_500ms,
	t_1000ms,
	t_2000ms,
	t_4000ms,
};

void BMP280Init() {

	// Specify BMP280 configuration
	// set pressure amd temperature output data rate
	// Configure the BMP280
	// Set T and P oversampling rates and sensor mode
	writeByte(BMP280_ADDRESS, BMP280_CTRL_MEAS, Tosr << 5 | Posr << 2 | Mode);
	// Set standby time interval in normal mode and bandwidth
	writeByte(BMP280_ADDRESS, BMP280_CONFIG, SBy << 5 | IIRFilter << 2);
	// Read and store calibration data
	uint8_t calib[24];
	readBytes(BMP280_ADDRESS, BMP280_CALIB00, 24, &calib[0]);
	dig_T1 = (uint16_t) (((uint16_t) calib[1] << 8) | calib[0]);
	dig_T2 = (int16_t) (((int16_t) calib[3] << 8) | calib[2]);
	dig_T3 = (int16_t) (((int16_t) calib[5] << 8) | calib[4]);
	dig_P1 = (uint16_t) (((uint16_t) calib[7] << 8) | calib[6]);
	dig_P2 = (int16_t) (((int16_t) calib[9] << 8) | calib[8]);
	dig_P3 = (int16_t) (((int16_t) calib[11] << 8) | calib[10]);
	dig_P4 = (int16_t) (((int16_t) calib[13] << 8) | calib[12]);
	dig_P5 = (int16_t) (((int16_t) calib[15] << 8) | calib[14]);
	dig_P6 = (int16_t) (((int16_t) calib[17] << 8) | calib[16]);
	dig_P7 = (int16_t) (((int16_t) calib[19] << 8) | calib[18]);
	dig_P8 = (int16_t) (((int16_t) calib[21] << 8) | calib[20]);
	dig_P9 = (int16_t) (((int16_t) calib[23] << 8) | calib[22]);
}

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of
// “5123” equals 51.23 DegC.
int32_t bmp280_compensate_T(int32_t adc_T) {
	int32_t var1, var2, T;
	var1 = ((((adc_T >> 3) - ((int32_t) dig_T1 << 1))) * ((int32_t) dig_T2))
			>> 11;
	var2 = (((((adc_T >> 4) - ((int32_t) dig_T1))
			* ((adc_T >> 4) - ((int32_t) dig_T1))) >> 12) * ((int32_t) dig_T3))
			>> 14;
	t_fine = var1 + var2;
	T = (t_fine * 5 + 128) >> 8;
	return T;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8
//fractional bits).
//Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
uint32_t bmp280_compensate_P(int32_t adc_P) {
	long long var1, var2, p;
	var1 = ((long long) t_fine) - 128000;
	var2 = var1 * var1 * (long long) dig_P6;
	var2 = var2 + ((var1 * (long long) dig_P5) << 17);
	var2 = var2 + (((long long) dig_P4) << 35);
	var1 = ((var1 * var1 * (long long) dig_P3) >> 8)
			+ ((var1 * (long long) dig_P2) << 12);
	var1 = (((((long long) 1) << 47) + var1)) * ((long long) dig_P1) >> 33;
	if (var1 == 0) {
		return 0;
		// avoid exception caused by division by zero
	}
	p = 1048576 - adc_P;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = (((long long) dig_P9) * (p >> 13) * (p >> 13)) >> 25;
	var2 = (((long long) dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((long long) dig_P7) << 4);
	return (uint32_t) p;
}
//The setup function is called once at startup of the sketch

int32_t readBMP280Pressure() {
	uint8_t rawData[3];  // 20-bit pressure register data stored here
	readBytes(BMP280_ADDRESS, BMP280_PRESS_MSB, 3, &rawData[0]);
	return (int32_t) (((int32_t) rawData[0] << 16 | (int32_t) rawData[1] << 8
			| rawData[2]) >> 4);
}

int32_t readBMP280Temperature() {
	uint8_t rawData[3];  // 20-bit pressure register data stored here
	readBytes(BMP280_ADDRESS, BMP280_TEMP_MSB, 3, &rawData[0]);
	return (int32_t) (((int32_t) rawData[0] << 16 | (int32_t) rawData[1] << 8
			| rawData[2]) >> 4);
}



