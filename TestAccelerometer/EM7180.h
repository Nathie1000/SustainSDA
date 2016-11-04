// EM7180.h

#ifndef _EM7180_h
#define _EM7180_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "ReadWriteByte.h"
#include "DefineAddress.h"

class EM7180 {
public:
	EM7180();
	~EM7180();
	void SensorsDetected();
	void EM7180InitState();
	void setupSensors();
	void configureOM();
	void readSensorFS(uint16_t EM7180FS, int adress, int paramIndex);
	void EM7180SetModuleAccFS(uint16_t moduleFS, uint16_t accelerometerFS, int adress);
	void EM7180SetIntInParam(uint8_t param, uint32_t paramValue);
	void EM7180Status();
	void sensorState();
	//bool setDLPFMode(const uint8_t mode);
};
#endif

