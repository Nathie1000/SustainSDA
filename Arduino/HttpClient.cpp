/*
 * HttpClient.cpp
 *
 *  Created on: 20 sep. 2016
 *      Author: Nathan
 */

#include "HttpClient.h"
#include "AtClient.h"
#include "WProgram.h"

const String HttpClient::CONTENT_TYPE_POST = "application/x-www-form-urlencoded";
const String HttpClient::CONTENT_TYPE_PLAIN_TEXT = "text/plain";
const String HttpClient::CONTENT_TYPE_JSON = "application/json";
const String HttpClient::CONTENT_TYPE_JAVASCRIPT = "application/javascript";

HttpClient::HttpClient(AtClient &at):
at(at)
{
}

bool HttpClient::querryBearer(int &status, String &ip){
	String rsp;
	if(at.execute("AT+SAPBR=2,1", rsp)){
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


bool HttpClient::openBearer(){
	return at.execute("AT+SAPBR=1,1");
}

bool HttpClient::closeBearer(){
	return at.execute("AT+SAPBR=0,1");
}

bool HttpClient::httpAction(bool method, int &status, int &length){
	String rsp;
	bool rtn = false;

	int originalTimeout = at.getTimeout();
	//Getting a response from a remote server might take a while.
	at.setTimeout(3000);
	if(at.execute("AT+HTTPACTION="+ String(method ? "0" : "1"), rsp)){
		rsp = rsp.replace("+HTTPACTION: ", "");
		//Parse response status.
		status = rsp.substring(rsp.indexOf(',')+1, rsp.lastIndexOf(',')).toInt();
		//Parse response length.
		length = rsp.substring(rsp.lastIndexOf(',')+1).toInt();
		rtn = true;
	}
	at.setTimeout(originalTimeout);
	return rtn;
}

String HttpClient::send(const String &url, const String &body, bool method, const String &contentType){
	//Start HTTP serves.
	if(!at.execute("AT+HTTPINIT")) return "";
	//Select a bearer. We always use 1.
	if(!at.execute("AT+HTTPPARA=\"CID\",1")) return "";
	//Set the URL.
	if(!at.execute("AT+HTTPPARA=\"URL\",\"" + String(url) +"\"")) return "";
	//Enable redirect, incase we need it.
	if(!at.execute("AT+HTTPPARA=\"REDIR\",1")) return "";

	//Check if method is POST. Only POST supports writing.
	if(!method){
		//Set the content type.
		if(!at.execute("AT+HTTPPARA=\"CONTENT\",\""+contentType+"\"")) return "";
		if(body.length() > 0){
			String rtn;
			//Open stream for writing.
			//We are writing the exact same number of bytes a we say we will so no need for the timeout.
			//It's set to max (12 sec) so we have all the time in the world.
			if(at.execute("AT+HTTPDATA=" + String(body.length()) + ",120000", rtn, "\r\nDOWNLOAD\r\n")){
				//Write body.
				at.execute(body, nullptr, 0);
				//We do not read response so we need to a little.
				delay(100);
			}
		}
	}

	int status;
	int length;
	String response;
	//Execute the HTTP action.
	if(httpAction(method, status, length)){
		if(at.execute("AT+HTTPREAD", response)){
			//Remove the head from the response.
			response = response.replace("+HTTPREAD: ", "");
			//Remove the response lenght, we already know how long it is.
			response = response.remove(0, String(length).length());
		}
	}

	//Kill HTTP service to save battery.
	if(!at.execute("AT+HTTPTERM")) return "";
	return response;
}

bool HttpClient::connect(){
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
			if(	at.execute("AT+SAPBR=3,1,\"Contype\",\"GPRS\"") &&
				at.execute("AT+SAPBR=3,1,\"APN\",\"www\"")
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

bool HttpClient::isConnected(){
	int status;
	String ip;
	if(querryBearer(status, ip)){
		return status == BearesStatus::CONNECTED;
	}
	return false;
}

bool HttpClient::close(){
	return closeBearer();
}

String HttpClient::post(const String &url, const String &body, const String &contentType){
	return send(url, body, false, contentType );
}

String HttpClient::get(const String &url){
	return send(url, "", true, "");
}

String HttpClient::getIp(){
	int status;
	String ip;
	querryBearer(status, ip);
	return ip;
}

