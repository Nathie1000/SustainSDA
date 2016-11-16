/**
 * @file LocationController.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 1 nov. 2016
 */

#include "LocationController.h"
#include <Arduino.h>
#include <TimeLib.h>
#include "PmtkClient.h"
#include "TaskBase.h"
#include "GsmClient.h"
#include "Debug.h"

LocationController *LocationController::instance = nullptr;

LocationController & LocationController::getInstance(){
	if(instance == nullptr){
		instance = new LocationController();
	}
	return *instance;
}

LocationController::LocationController():
TaskBase(3, "Location Task"),
pmtk(PmtkClient::getInstance()),
gsm(GsmClient::getInstance()),
latitude(0.0),
longitude(0.0),
timer(3000)
{

}

void LocationController::run(){
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
		PRINTLN("No AT or PMTK device found, Location Task suspended.");
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

void LocationController::onTimeout(Timer & timer){
	flag.set();
}

float LocationController::getLatitude(){
	return latitude;
}

float LocationController::getLongitude(){
	return longitude;
}

int LocationController::getSeconds(){
	return second();
}

int LocationController::getMinuts(){
	return minute();
}

int LocationController::getHours(){
	return hour();
}

int LocationController::getDay(){
	return day();
}

int LocationController::getMonth(){
	return month();
}

int LocationController::getYear(){
	return year();
}

void LocationController::addLocationListener(LocationListener &locationListener){
	locationListeners.add(&locationListener);
}
