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

void readSENtralDevices();
void SENtralPassThroughMode();	
float uint32_reg_to_float(uint8_t *buf);
void readSENtralAccelData(int16_t * destination);
void readSENtralQuatData(float * destination);
void readSENtralGyroData(int16_t * destination);
void readSENtralMagData(int16_t * destination);
int16_t readSENtralBaroData();
int16_t readSENtralTempData();

#endif

