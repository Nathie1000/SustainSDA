// EEPROM.h

#ifndef _EEPROM_h
#define _EEPROM_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ReadWriteByte.h"
#include "DefineAddress.h"

// Specify sensor full scale
extern float aRes, gRes, mRes;      // scale resolutions per LSB for the sensors
extern float magCalibration[3];  // Factory mag calibration and mag bias
extern float q[4];

void initEEPROM();
void initMPU9250(); 
void initAK8963(float * destination);
void magcalMPU9250(float * dest1, float * dest2);
void getAverageValue(uint8_t rawData[], int16_t deviceAverage[], int16_t deviceAverage2[]);
void MPU9250SelfTest(float * destination);
void getMres(); void getGres(); void getAres();
void accelgyrocalMPU9250(float * dest1, float * dest2);
void readMagData(int16_t * destination);
void readAccelData(int16_t * destination);
void readGyroData(int16_t * destination); 
void MadgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz, float deltat);

#endif

