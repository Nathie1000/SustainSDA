// BMP280.h

#ifndef _BMP280_h
#define _BMP280_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "DefineAddress.h"
#include "ReadWriteByte.h"
//static of extern namespace classe
class BMP280 {
public:
	BMP280();
	~BMP280();
	void BMP280Init();
	int32_t readBMP280Temperature();
	int32_t readBMP280Pressure();
	int32_t BMP280_compensate_T(int32_t adc_T);
	uint32_t BMP280_compensate_P(int32_t adc_P);
};

#endif

