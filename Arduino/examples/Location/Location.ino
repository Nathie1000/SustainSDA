/**
 * @file MotionSensor.ino
 *
 * @author Nathan Schaaphuizen
 * @date 30 nov. 2016
 */
#include <Arduino.h>
#include <SustainWork.h>

//Example class on how to get the Location data.
class LocationHandler : public LocationListener{
public:
	LocationHandler(){
		//Register this object with the Motion Controller.
		LocationTimeControler::getInstance().addLocationListener(*this);
	}

	//Callback function called by the LocationTimeControler when a new location is available.
	//Every time this function is called the Time is also synchronized with a remote device.
	void onLocationFound(float latitude, float longitude) override{
		PRINTLN("lat: " + String(latitude,6) + " long: " + String(longitude,6));

		LocationTimeControler &ltc = LocationTimeControler::getInstance();
		//The time function will be kept up to date by the Teensy RTC.
		//Every time this function is called the Time is synchronized.
		PRINTLN(String() + ltc.getHours() + ":" + ltc.getMinutes() + ":" + ltc.getSeconds())
	}

};


void setup(){
	//Start debugging. This line will automatically be removed if debugging is disabled.
	DEBUG_BEGIN(9600);

	//Create a new instance that handles the Motion events.
	//This object is called back by the frame work an must therefore exits in dynamic memory.
	//Do not allocate anything on the stack for it will be destroyed.
	new LocationHandler;

	//Start the scheduler, this function does not return.
	TaskBase::startAllTasks();
}

void loop(){
	//Not used.
}
