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
	String rsp;
	lastResponseStatus = -1;
	//Getting a response from a remote server might take a while.
	if(at.execute("AT+HTTPACTION="+ String(method ? "0" : "1"))){
		rsp = at.scan(120000);
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
	at.execute("AT+HTTPTERM");
	//Start HTTP serves.
	if(!at.execute("AT+HTTPINIT")){ PRINTLN("Failed to initialize HTTP service.") return "";}
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
				//We do not read the response so we need to a little waiting.
				TaskBase::sleep(100);
			}
		}
	}

	int length;
	String response;
	//Execute the HTTP action.
	if(httpAction(method, lastResponseStatus, length)){
		if(at.execute("AT+HTTPREAD", response)){
			//Remove the head from the response.
			response = response.replace("+HTTPREAD: ", "");
			//Remove the response lenght, we already know how long it is.
			response = response.remove(0, String(length).length());
		}
	}

	//Kill HTTP service to save battery.
	if(!at.execute("AT+HTTPTERM")) {PRINTLN("Failed to terminate HTTP service."); return "";}
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
