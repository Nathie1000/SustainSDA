/**
 * @file Bearer.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 26 okt. 2016
 */
#include "Bearer.h"
#include "AtClient.h"

bool Bearer::querryBearer(int &status, String &ip){
	String rsp;
	if(at.execute(String("AT+SAPBR=2,") + bearer, rsp)){
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
	return at.execute(String("AT+SAPBR=1,") + bearer);
}

bool Bearer::closeBearer(){
	return at.execute(String("AT+SAPBR=0,") + bearer);
}

Bearer::Bearer(AtClient &at, int id, const String &apn, const String &contype):
bearer(id),
apn(apn),
contype(contype),
at(at)
{

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
			if(	at.execute(String("AT+SAPBR=3,") + bearer +",\"Contype\",\""+ contype +"\"") &&
				at.execute(String("AT+SAPBR=3,") + bearer +",\"APN\",\""+ apn +"\"")
			){
				int originalTimeout = at.getTimeout();
				//Connecting can take a while.
				at.setTimeout(3000);
				rtn = openBearer();
				at.setTimeout(originalTimeout);
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
			int originalTimeout = at.getTimeout();
			at.setTimeout(3000);
			rtn = closeBearer();
			at.setTimeout(originalTimeout);
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

