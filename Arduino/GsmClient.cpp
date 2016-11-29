/**
 * @file GsmClient.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 26 okt. 2016
 */

#include "GsmClient.h"
#include <Arduino.h>
#include "Bearer.h"
#include "Debug.h"
#include "TaskBase.h"
#include "AtClient.h"
#include "ArrayList.h"
#include "PollControler.h"

GsmClient *GsmClient::instance = nullptr;

GsmClient & GsmClient::getInstance(){
	if(instance == nullptr){
		instance = new GsmClient(AtClient::getInstance());
	}
	return *instance;
}

GsmClient::GsmClient(AtClient &at):
Bearer(at,2,"CMNET","GPRS")
{
}

bool GsmClient::setPinCode(const String &pin){
	return at.execute("AT+CPIN="+pin);
}

GsmClient::PinState GsmClient::getPinState(){
	String rps;
	if(at.execute("AT+CPIN?", rps)){
		rps = rps.replace("+CPIN: ", "");
		if(rps.equals("READY")){
			return PinState::READY;
		}
		else if(rps.equals("SIM PIN")){
			return PinState::SIM_PIN;
		}
		else if(rps.equals("SIM PUK")){
			return PinState::SIM_PUK;
		}
		else if(rps.equals("PH_SIM PIN")){
			return PinState::PH_SIM_PIN;
		}
		else if(rps.equals("PH_SIM PUK")){
			return PinState::PH_SIM_PUK;
		}
	}
	return PinState::ERROR;
}

int GsmClient::getSignalQuality(){
	String rsp;
	if(at.execute("AT+CSQ", rsp)){
		rsp = rsp.replace("+CSQ: ", "");
		return rsp.substring(0, rsp.indexOf(",")).toInt();
	}
	return -1;
}

bool GsmClient::getLocationAndTime(float &latitude, float &longitude, String &date, String &time){
	bool rtn = false;
	//Open the bearer if it wasn't open already.
	for(int i=0; i<3 && !connect(); i++){
		TaskBase::sleep(1000);
	}

	String rsp;
	int oldTimeout = at.getTimeout();
	at.setTimeout(5000);
	if(at.execute(String("AT+CIPGSMLOC=1,")+getBearerId(), rsp)){
		rsp = rsp.replace("+CIPGSMLOC: ","");
		//Get location code
		int locationCode = rsp.substring(0,rsp.indexOf(',')).toInt();
		rsp = rsp.remove(0, rsp.indexOf(',') +1);
		if(locationCode == 0){
			//Get longitude
			longitude = rsp.substring(0,rsp.indexOf(',')).toFloat();
			rsp = rsp.remove(0, rsp.indexOf(',') +1);
			//Get latitude
			latitude = rsp.substring(0,rsp.indexOf(',')).toFloat();
			rsp = rsp.remove(0, rsp.indexOf(',')+1);
			//Get date
			date = rsp.substring(0,rsp.indexOf(','));
			rsp = rsp.remove(0, rsp.indexOf(',')+1);
			//Get time
			time = rsp;
			rtn = true;
		}
	}
	else{
		PRINTLN("Error getting location and time.");
	}
	at.setTimeout(oldTimeout);
	return rtn;
}

bool GsmClient::sendSms(const String &number, const String text){
	return at.execute("AT+CMGF=1") && at.execute("AT+CMGS="+number+"\r\n"+text+"\032");
}

bool GsmClient::getPhoneNumber(String &number){
	String rsp;
	if(at.execute("AT+CNUM", rsp)){
		//TODO: parse this.
		PRINTLN("N: " + rsp);
		return true;
	}
	return false;
}
