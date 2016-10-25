// SENtral.h

#ifndef _SENTRAL_h
#define _SENTRAL_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ReadWriteByte.h"
#include "DefineAddress.h"
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
};

#endif

