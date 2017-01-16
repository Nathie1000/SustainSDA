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
	at.send("AT+CPIN="+pin);
	String rsp = at.scan(AtClient::AT_OK);
	return rsp.indexOf(AtClient::AT_OK) != -1;
}

GsmClient::PinState GsmClient::getPinState(){
	at.send("AT+CPIN?");
	String rsp = at.scan(AtClient::AT_OK);
	rsp = rsp.replace("AT+CPIN?", "");

	if(at.isOk(rsp)){
		rsp = rsp.replace("+CPIN: ", "");
		if(rsp.indexOf("READY") != -1){
			return PinState::READY;
		}
		else if(rsp.indexOf("SIM PIN") != -1){
			return PinState::SIM_PIN;
		}
		else if(rsp.indexOf("SIM PUK") != -1){
			return PinState::SIM_PUK;
		}
		else if(rsp.indexOf("PH_SIM PIN") != -1){
			return PinState::PH_SIM_PIN;
		}
		else if(rsp.indexOf("PH_SIM PUK") != -1){
			return PinState::PH_SIM_PUK;
		}
	}
	return PinState::ERROR;
}

int GsmClient::getSignalQuality(){
	at.send("AT+CSQ");
	String rsp = at.scan(AtClient::AT_OK);
	rsp = rsp.replace("AT+CSQ", "");
	if(at.isOk(rsp)){
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

	at.send(String("AT+CIPGSMLOC=1,")+getBearerId());
	String rsp = at.scan(AtClient::AT_OK, 3000);
	rsp.replace(String("AT+CIPGSMLOC=1,")+getBearerId(), "");

	if(at.isOk(rsp)){
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
	return rtn;
}

bool GsmClient::sendSms(const String &number, const String text){
	at.send("AT+CMGF=1");
	String rsp = at.scan(AtClient::AT_OK);
	if(at.isOk(rsp)){
		at.send("AT+CMGS="+number+"\r\n"+text+"\032");
		rsp = at.scan(AtClient::AT_OK);
		return at.isOk(rsp);
	}
	return false;
}

bool GsmClient::getPhoneNumber(String &number){
	at.send("AT+CNUM");
	String rsp = at.scan(AtClient::AT_OK);
	rsp.replace("AT+CNUM", "");
	if(at.isOk(rsp)){
		rsp = rsp.replace("\r\n", "");
		number = rsp;
		return true;
	}
	return false;
}

bool GsmClient::getCcid(String &ccid){
	at.send("AT+CCID");
	String rsp = at.scan(AtClient::AT_OK);
	rsp = rsp.replace("AT+CCID", "");
	if(at.isOk(rsp)){
		rsp = rsp.replace("\r\n", "");
		ccid = rsp;
		return true;
	}
	return false;
}
