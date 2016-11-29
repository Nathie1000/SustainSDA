/**
 * @file LocationTimeController.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 1 nov. 2016
 */

#include "LocationTimeController.h"

#include <Arduino.h>
#include <TimeLib.h>
#include "PmtkClient.h"
#include "TaskBase.h"
#include "GsmClient.h"
#include "Debug.h"

LocationTimeController *LocationTimeController::instance = nullptr;

LocationTimeController & LocationTimeController::getInstance(){
	if(instance == nullptr){
		instance = new LocationTimeController();
	}
	return *instance;
}

/**
 * Helper function to server a a sync provider for the Teensy RTC.
 * @return The elapsed time in ms.
 */
time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

LocationTimeController::LocationTimeController():
TaskBase(3, "LocationTime Task"),
pmtk(PmtkClient::getInstance()),
gsm(GsmClient::getInstance()),
latitude(0.0),
longitude(0.0),
timer(3000)
{
	setSyncProvider(getTeensy3Time);
}

void LocationTimeController::run(){
	State state = USE_NONE;
	PRINTLN("-----------------Location Task Start-----------");
	//Test GPS first, this is the better option so we should use it if we can.
	//Test if device is already connected.
	if(false){
		//If not open it.
		if(false){
			state = USE_GPS;
			timer.start();
		}
	}
	//The GPRS is is a lesser alternative option.
	//Test if device is already connected and try to connect if not.
	else if(gsm.isDeviceOpen() || gsm.openDevice()){
		state = USE_GSM;
		timer.start();
	}
	//No device found.
	else{
		PRINTLN("No AT or PMTK device found, Location Time Task suspended.");
		suspend(); //Task will end here.
	}

	while(true){
		flag.wait();
		if(state == USE_GSM){
			//Parse date and time.
			String date, time;
			//Time
			gsm.getLocationAndTime(latitude, longitude, date, time);
			int hour = time.substring(0, time.indexOf(':')).toInt();
			time.remove(0, time.indexOf(':'));
			int min = time.substring(0, time.indexOf(':')).toInt();
			time.remove(0, time.indexOf(':'));
			int sec = time.substring(0, time.indexOf(':')).toInt();
			//Date
			int year = date.substring(0, date.indexOf('/')).toInt();
			date.remove(0, date.indexOf('/'));
			int month = date.substring(0, date.indexOf('/')).toInt();
			date.remove(0, date.indexOf('/'));
			int day = date.substring(0, date.indexOf('/')).toInt();
			//Update date and time.
			setTime(hour, min, sec, day, month, year);
		}
		else if(state == USE_GPS){
			//TODO get GPS data.
		}

		for(LocationListener *locationListener : locationListeners){
			locationListener->onLocationFound(latitude, longitude);
		}
	}
}

void LocationTimeController::onTimeout(Timer & timer){
	flag.set();
}

float LocationTimeController::getLatitude(){
	return latitude;
}

float LocationTimeController::getLongitude(){
	return longitude;
}

int LocationTimeController::getSeconds(){
	return second();
}

int LocationTimeController::getMinutes(){
	return minute();
}

int LocationTimeController::getHours(){
	return hour();
}

int LocationTimeController::getDay(){
	return day();
}

int LocationTimeController::getMonth(){
	return month();
}

int LocationTimeController::getYear(){
	return year();
}

void LocationTimeController::addLocationListener(LocationListener &locationListener){
	locationListeners.add(&locationListener);
}
