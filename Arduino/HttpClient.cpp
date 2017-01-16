/**
 * @file HttpClient.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 20 sep. 2016
 */

#include "HttpClient.h"
#include "AtClient.h"
#include "WProgram.h"
#include "TaskBase.h"

#include "Debug.h"

const String HttpClient::CONTENT_TYPE_POST = "application/x-www-form-urlencoded";
const String HttpClient::CONTENT_TYPE_PLAIN_TEXT = "text/plain";
const String HttpClient::CONTENT_TYPE_JSON = "application/json";
const String HttpClient::CONTENT_TYPE_JAVASCRIPT = "application/javascript";

HttpClient::HttpClient(AtClient &at):
Bearer(at,1,"www","GPRS"),
lastResponseStatus(0)
{
}


bool HttpClient::httpAction(bool method, int &status, int &length){

	lastResponseStatus = -1;
	//Getting a response from a remote server might take a while.
	at.send("AT+HTTPACTION="+ String(method ? "0" : "1"));
	String rsp = at.scan("+HTTPACTION: ", 120*1000);

	TaskBase::sleep(1000);
	rsp.replace("AT+HTTPACTION="+ String(method ? "0" : "1"), "");
	if(at.isOk(rsp)){
		if(rsp.length() > 0 && rsp.indexOf("+HTTPACTION: ") != -1){
			rsp = rsp.replace("+HTTPACTION: ", "");
			//Parse response status.
			status = rsp.substring(rsp.indexOf(',')+1, rsp.lastIndexOf(',')).toInt();
			//Parse response length.
			length = rsp.substring(rsp.lastIndexOf(',')+1).toInt();
			return true;
		}
	}
	return false;
}

String HttpClient::send(const String &url, const String &body, bool method, const String &contentType){
	//Terminate HTTP serves if it was still on.
	//This action may fail if service was already terminated.
	at.send("AT+HTTPTERM");
	at.scan(AtClient::AT_OK);

	//Start HTTP serves.
	at.send("AT+HTTPINIT");
	String rsp = at.scan(AtClient::AT_OK);
	if(!at.isOk(rsp)){ PRINTLN("Failed to initialize HTTP service.") return "";}

	//Select a bearer. We always use 1.
	at.send("AT+HTTPPARA=\"CID\","+String(getBearerId()));
	rsp = at.scan(AtClient::AT_OK);
	if(!at.isOk(rsp)) return "";

	//Set the URL.
	at.send("AT+HTTPPARA=\"URL\",\"" + String(url) +"\"");
	rsp = at.scan(AtClient::AT_OK);
	if(!at.isOk(rsp)) return "";

	//Enable redirect, incase we need it.
	at.send("AT+HTTPPARA=\"REDIR\",1");
	rsp = at.scan(AtClient::AT_OK);
	if(!at.isOk(rsp)) return "";

	//Check if method is POST. Only POST supports writing.
	if(!method){
		//Set the content type.
		at.send("AT+HTTPPARA=\"CONTENT\",\""+contentType+"\"");
		rsp = at.scan(AtClient::AT_OK);
		if(!at.isOk(rsp)) return "";
		if(body.length() > 0){
			String rtn;
			//Open stream for writing.
			//We are writing the exact same number of bytes a we say we will so no need for the timeout.
			//It's set to max (12 sec) so we have all the time in the world.
			at.send("AT+HTTPDATA=" + String(body.length()) + ",120000");
			rsp = at.scan("\r\nDOWNLOAD\r\n");

			if(rsp.indexOf("\r\nDOWNLOAD\r\n") != -1){
				//Write body.
				at.send(body);
				at.scan(AtClient::AT_OK);
				//We do not read the response so we need to a little waiting.
				//TaskBase::sleep(100);
			}
		}
	}

	int length;
	String response;
	//Execute the HTTP action.
	if(httpAction(method, lastResponseStatus, length)){
		at.send("AT+HTTPREAD");
		response = at.scan(AtClient::AT_OK, 3000);
		response = response.replace("AT+HTTPREAD", "");
		if(at.isOk(response)){
			//Remove the head from the response.
			response = response.replace("+HTTPREAD: " + String(length) + "\r\n", "");
			//Remove extra enter.
			response = response.remove(0,2);
		}
	}
	//Kill HTTP service to save battery.
	at.send("AT+HTTPTERM");
	rsp = at.scan(AtClient::AT_OK);
	if(!at.isOk(rsp)) {PRINTLN("Failed to terminate HTTP service.");}
	return response;
}

String HttpClient::post(const String &url, const String &body, const String &contentType){
	return send(url, body, false, contentType );
}

String HttpClient::get(const String &url){
	return send(url, "", true, "");
}

int HttpClient::getStatus(){
	return lastResponseStatus;
}
