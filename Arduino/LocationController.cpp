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
longitude(0.0)
{

}

void LocationController::run(){
	PRINTLN("-----------------Location Task Start-----------");
	//Test GPS first, this is the better option so we should use it if we can.
	//Test if device is already connected.
	if(false){
		//If not open it.
		if(false){
		}
	}
	//The GPRS is is a lesser alternative option.
	//Test if device is already connected and try to connect if not.
	else if(gsm.isDeviceOpen() || gsm.openDevice()){
		gsm.addGsmListener(*this);
	}
	//No device found.
	else{
		PRINTLN("No AT or PMTK device found, Location Task suspended.");
		suspend(); //Task will end here.
	}

	while(true){
		flag.wait();
		for(LocationListener *locationListener : locationListeners){
			locationListener->onLocationFound(latitude, longitude);
		}
	}
}

void LocationController::onLocationAndDateTimeFound(float latitude, float longitude, const String &date , const String &time){
	//Parse date and time.
	String lDate, lTime;
	int hour = lTime.substring(0, lTime.indexOf(':')).toInt();
	lTime.remove(0, lTime.indexOf(':'));
	int min = lTime.substring(0, lTime.indexOf(':')).toInt();
	lTime.remove(0, time.indexOf(':'));
	int sec = lTime.substring(0, lTime.indexOf(':')).toInt();

	int year = lDate.substring(0, lDate.indexOf('/')).toInt();
	lDate.remove(0, lDate.indexOf('/'));
	int month = lDate.substring(0, lDate.indexOf('/')).toInt();
	lDate.remove(0, time.indexOf('/'));
	int day = lDate.substring(0, lDate.indexOf('/')).toInt();

	setTime(hour, min, sec, day, month, year);
	this->latitude = latitude;
	this->longitude = longitude;

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
