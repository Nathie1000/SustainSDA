/**
 * @file Bearer.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 26 okt. 2016
 */
#include "Bearer.h"
#include "AtClient.h"

bool Bearer::querryBearer(int &status, String &ip){
	at.send(String("AT+SAPBR=2,") + bearer);
	String rsp = at.scan(AtClient::AT_OK);
	rsp = rsp.replace(String("AT+SAPBR=2,") + bearer, "");
	if(at.isOk(rsp)){
		//Parse the bearer id.
		rsp = rsp.remove(0, rsp.indexOf(',') + 1);
		//Parse the status.
		status = rsp.substring(0,1).toInt();
		//Parse the ip.
		rsp = rsp.remove(0, rsp.indexOf(',') + 1);
		ip = rsp.replace('"', "");
		return true;
	}
	return false;
}

bool Bearer::openBearer(){
	at.send(String("AT+SAPBR=1,") + bearer);
	String rsp = at.scan(AtClient::AT_OK, 3000);
	return at.isOk(rsp);
}

bool Bearer::closeBearer(){
	at.send(String("AT+SAPBR=0,") + bearer);
	String rsp = at.scan(AtClient::AT_OK, 3000);
	return at.isOk(rsp);
}

Bearer::Bearer(AtClient &at, int id, const String &apn, const String &contype):
bearer(id),
apn(apn),
contype(contype),
at(at)
{

}

bool Bearer::openDevice(){
	return at.connect();
}

bool Bearer::isDeviceOpen(){
	return at.isConnected();
}

int Bearer::getBearerId(){
	return bearer;
}

String Bearer::getApn(){
	return apn;
}

String Bearer::getContType(){
	return contype;
}

bool Bearer::connect(){
	int status;
	String ip;
	bool rtn = false;
	//Question the status of the bearer.
	if(querryBearer(status, ip)){
		//If it's already connected we do not need to do anything.
		if(status == BearesStatus::CONNECTED || status == BearesStatus::CONNECTING){
			rtn = true;
		}
		//If it's closed we need to open it.
		else if(status == BearesStatus::CLOSED || status == BearesStatus::CLOSING){
			//Set the connection type and the access point name.
			at.send(String("AT+SAPBR=3,") + bearer +",\"Contype\",\""+ contype +"\"");
			String rsp = at.scan(AtClient::AT_OK);
			if(at.isOk(rsp)){
				at.send(String("AT+SAPBR=3,") + bearer +",\"APN\",\""+ apn +"\"");
				rsp = at.scan(AtClient::AT_OK);
				if(at.isOk(rsp)){
					rtn = openBearer();
				}
			}
		}
	}
	return rtn;
}

bool Bearer::isConnected(){
	int status;
	String ip;
	if(querryBearer(status, ip)){
		return status == BearesStatus::CONNECTED;
	}
	return false;
}

bool Bearer::disonnect(){
	int status;
	String ip;
	bool rtn = false;
	//Question the status of the bearer.
	if(querryBearer(status, ip)){
		//If it's connected we do not need to close it.
		if(status == BearesStatus::CONNECTED || status == BearesStatus::CONNECTING){
			rtn = closeBearer();
		}
		//If it's already closed we do not need to do anything.
		else if(status == BearesStatus::CLOSED || status == BearesStatus::CLOSING){
			rtn = true;
		}
	}
	return rtn;
}

String Bearer::getIp(){
	int status;
	String ip;
	querryBearer(status, ip);
	return ip;
}

