/**
 * @file LocationController.h
 *
 * @author Nathan Schaaphuizen
 * @date 1 nov. 2016
 */

#ifndef LOCATIONCONTROLLER_H_
#define LOCATIONCONTROLLER_H_

#include <Arduino.h>
#include "TaskBase.h"
#include "PmtkClient.h"
#include "ArrayList.h"
#include "GsmClient.h"
#include "Timer.h"
#include "Flag.h"

class LocationListener{
public:
	virtual ~LocationListener(){};
	virtual void onLocationFound(float latitude, float longitude) = 0;
};

class LocationController : public TaskBase, public TimerListener{
private:
	static LocationController * instance;
	enum State{
		USE_NONE,
		USE_GPRS,
		USE_GPS,
	};

	PmtkClient &pmtk;
	GsmClient &gsm;
	ArrayList<LocationListener*> locationListeners;
	State state;
	Timer gpsPollTimer;
	Flag locationFlag;
	float latitude;
	float longitude;

	LocationController();

public:
	static LocationController & getInstance();

	void run() override;
	void onTimeout(Timer &timer) override;

	float getLatitude();
	float getLongitude();

	void addLocationListener(LocationListener &locationListener);

};



#endif /* LOCATIONCONTROLLER_H_ */
