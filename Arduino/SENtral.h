// SENtral.h

#ifndef _SENTRAL_h
#define _SENTRAL_h

#include <Arduino.h>

class SENtral {
public:
	SENtral();
	~SENtral();
	void readSENtralDevices();
	void SENtralPassThroughMode();
	float uint32_reg_to_float(uint8_t *buf);
	void readSENtralData(int16_t * destination, int deviceAddress);
	void readSENtralQuatData(float * destination);
	int16_t readSENtralData(int deviceAddress);
	void newData(int16_t count[], int address, float &x, float &y, float &z, float factor);
	void error();
};

#endif //_SENTRAL_h

