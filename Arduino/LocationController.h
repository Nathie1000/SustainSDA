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
#include "Flag.h"

class LocationListener{
public:
	virtual ~LocationListener(){};
	virtual void onLocationFound(float latitude, float longitude) = 0;
};

class LocationController : public TaskBase, public GsmListener{
private:
	static LocationController * instance;

	PmtkClient &pmtk;
	GsmClient &gsm;
	ArrayList<LocationListener*> locationListeners;
	Flag flag;
	float latitude;
	float longitude;

	LocationController();

public:
	static LocationController & getInstance();

	void run() override;
	void onLocationAndDateTimeFound(float latitude, float longitude, const String &date , const String &time);

	float getLatitude();
	float getLongitude();

	int getSeconds();
	int getMinuts();
	int getHours();

	int getDay();
	int getMonth();
	int getYear();

	void addLocationListener(LocationListener &locationListener);
};



#endif /* LOCATIONCONTROLLER_H_ */
