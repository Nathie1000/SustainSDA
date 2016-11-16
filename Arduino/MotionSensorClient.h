/**
 * @file MotionSensorClient.h
 *
 * @author Nathan Schaaphuizen
 * @date 14 nov. 2016
 */

#ifndef MOTIONSENSORCLIENT_H_
#define MOTIONSENSORCLIENT_H_

#include "Sensor.h"
#include "ArrayList.h"
#include "BMP280.h"
#include "EM7180.h"
#include "SENtral.h"
#include <i2c_t3.h>
#include "Eeprom.h"

class MotionSensorListener{
public:
	struct Motion{
		float ax, ay, az;
		float gx, gy, gz;
		float mx, my, mz;
	};

	virtual ~MotionSensorListener(){}
	virtual void onMotion(const Motion &newMotion) = 0;
};

class MotionSensorClient : public Sensor{
private:
	ArrayList<MotionSensorListener*> motionListeners;
	BMP280 bmp;
	Eeprom eeprom;
	EM7180 em7180;
	SENtral sentral;
	int lastPollTime;
	bool isOpen;

public:
	MotionSensorClient(int groundPin, int volt3V3Pin, i2c_pins ic2Pins);
	bool openDevice();
	bool isDeviceOpen();

	void update() override;
	void addMotionListener(MotionSensorListener &motionListener);
};

#endif /* MOTIONSENSORCLIENT_H_ */
