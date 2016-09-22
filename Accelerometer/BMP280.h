/*
 * BMP280.h
 *
 *  Created on: Sep 19, 2016
 *      Author: Bianca
 */

#ifndef BMP280_H_
#define BMP280_H_

#include "Byte.h"

#define BMP280_PRESS_MSB  0xF7
#define BMP280_CONFIG     0xF5
#define BMP280_CTRL_MEAS  0xF4
#define BMP280_TEMP_MSB   0xFA
#define BMP280_ADDRESS	  0x76   // Address of BMP280 altimeter when ADO = 0

#define BMP280_CALIB00    0x88

// BMP280 compensation parameters
static uint16_t dig_T1, dig_P1;
static int16_t dig_T2, dig_T3, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;

// Specify BMP280 configuration
static uint8_t Posr, Tosr , Mode, IIRFilter, SBy;     // set pressure amd temperature output data rate

// t_fine carries fine temperature as global value for BMP280
static int32_t t_fine;

void BMP280Init();
int32_t bmp280_compensate_T(int32_t adc_T);
uint32_t bmp280_compensate_P(int32_t adc_P);

int32_t readBMP280Pressure();
int32_t readBMP280Temperature();

#endif /* BMP280_H_ */
