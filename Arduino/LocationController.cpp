/**
 * @file LocationController.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 1 nov. 2016
 */

#include "LocationController.h"
#include <Arduino.h>
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
state(State::USE_NONE),
gpsPollTimer(3000),
latitude(0.0),
longitude(0.0)
{
	gpsPollTimer.addTimerListener(*this);
}

void LocationController::run(){
	PRINTLN("-----------------Location Task Start-----------");
	//Test GPS first, this is the better option so we should use it if we can.
	//Test if device is already connected.
	if(false){
		//If not open it.
		if(false){
			state = State::USE_GPS;
		}
	}
	//The GPRS is is a lesser alternative option.
	//Test if device is already connected.
	else if(!gsm.isDeviceOpen()){
		//If not open it.
		if(gsm.openDevice()){
			state = State::USE_GPRS;
			gpsPollTimer.start();
		}
	}

	//See if we have a device ready to use.
	if(state == State::USE_NONE){
		PRINTLN("No AT or PMTK device found, task suspended.");
		suspend(); //Task will end here.
	}

	while(true){
		locationFlag.wait();
		if(state == State::USE_GPRS){
			//We ignore the date and time;
			String date, time;
			if(gsm.getLocationAndTime(longitude, latitude, date, time)){
				for(LocationListener *locationListener : locationListeners){
					locationListener->onLocationFound(latitude, longitude);
				}
			}
		}
		if(state == State::USE_GPS){
			//XXX Properly not useful.
			for(LocationListener *locationListener : locationListeners){
				locationListener->onLocationFound(latitude, longitude);
			}
		}
	}
}

void LocationController::onTimeout(Timer &timer){
	locationFlag.set();
}

float LocationController::getLatitude(){
	return latitude;
}

float LocationController::getLongitude(){
	return longitude;
}

void LocationController::addLocationListener(LocationListener &locationListener){
	locationListeners.add(&locationListener);
}

