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
class EEPROM {
public:
	// Specify sensor full scale
	float magCalibration[3] = { 0, 0, 0 };  // factory mag calibration and mag bias
	float q[4] = { 1.0f, 0.0f, 0.0f, 0.0f };    // vector to hold quaternion
	float aRes, gRes, mRes;

	EEPROM();
	~EEPROM();
	void initEEPROM();
	void initMPU9250();
	void initAK8963(float * destination);
	void magcalMPU9250(float * dest1, float * dest2);
	void getAverageValue(uint8_t rawData[], int16_t deviceAverage[], int16_t deviceAverage2[]);
	void MPU9250SelfTest(float * destination);
	void getMagResult(); 
	void getGyroResult(); 
	void getAccelResult();
	void accelGyroCalMPU9250(float * dest1, float * dest2);
	void readMagData(int16_t * destination);
	void readData(int16_t * destination, int deviceAddress);
	void MadgwickQuaternionUpdate(float accelX, float accelY, float accelZ, float gyroX, float gyroY, float gyroZ, float magX, float magY, float magZ, float deltat);
	float calculateSoftwareYPR(String rotation);

private:
	uint8_t mMode = 0x02;        // 2 for 8 hz, 6 for 100 hz continuous magnetometer data read
};
#endif

