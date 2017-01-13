/**
 * @file AtClient.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 16 sep. 2016
 */

#include "AtClient.h"
#include "WProgram.h"
#include "TaskBase.h"
#include "math.h"
#include "Debug.h"

AtClient *AtClient::instance = nullptr;
String AtClient::voidBuffer = "";
const String AtClient::AT_OK = "\r\nOK\r\n";
const String AtClient::AT_ERROR = "\r\nERROR\r\n";

AtClient & AtClient::getInstance(){
	if(instance == nullptr){
		instance = new AtClient(Serial3, 9600);
	}
	return *instance;
}

AtClient::AtClient(HardwareSerial &serial, int baudrate, int timeout):
serial(serial),
timeout(timeout)
{
	serial.begin(baudrate);
	serial.setTimeout(5);
}

AtClient::~AtClient(){
	serial.end();
}

bool AtClient::connect(){
	for(int i = 0; i<3; i++){
		send("AT");
		String rsp = scan(AT_OK);
		if(isOk(rsp)){
			break;
		}
	}
	send("ATE0");
	String rsp = scan(AT_OK);
	return isOk(rsp);
}

bool AtClient::isConnected(){
	send("AT");
	String rsp = scan(AT_OK);
	return isOk(rsp);
}

bool AtClient::reset(){
	send("ATZ");
	String rsp = scan(AT_OK);
	return isOk(rsp);
}

void AtClient::send(const String& atCommand){
	mutex.lock();
	serial.clear();
	if(atCommand.length() > 0){
		serial.println(atCommand);
	}
	//Wait for the serial buffer to be emptied so we know the command has been send.
	serial.flush();
	mutex.unlock();
}

String AtClient::scan(int timeout){
	mutex.lock();
	const int localTimeout = 10;
	int localTime = 0;
	String rsp = "";
	while(localTime < timeout){
		if(serial.available() > 0){
			//Read the buffer.
			rsp += serial.readString();
		}
		TaskBase::sleep(localTimeout);
		localTime += localTimeout;
	}
	mutex.unlock();
	return rsp;
}

String AtClient::scan(const String& until, int timeout){
	mutex.lock();
	const int localTimeout = 10;
	int localTime = 0;
	//Read the buffer.
	String rsp = "";
	while(localTime < timeout){
		if(serial.available() > 0){
			rsp += serial.readString();
		}
		if(rsp.indexOf(until) != -1){
			break;
		}
		TaskBase::sleep(localTimeout);
		localTime += localTimeout;
	}
	mutex.unlock();
	return rsp;
}

bool AtClient::isOk(String& response){
	bool isOk = response.indexOf(AT_OK) != -1;
	if(isOk){
		response.replace(AT_OK, "");
	}
	return isOk;
}

bool AtClient::isError(String& response){
	bool isError = response.indexOf(AT_ERROR) != -1;
	if(isError){
		response.replace(AT_ERROR, "");
	}
	return isError;
}

//void AtClient::setTimeout(int timeout){
//	this->timeout = timeout;
//}
//
//int AtClient::getTimeout(){
//	return timeout;
//}
//
//int AtClient::execute(const String& atCommand, char *buffer, int length){
//	//Lock the serial resource.
//	mutex.lock();
//	//Clear Rx buffer to remove left overs from previous serial use.
//	serial.clear();
//	if(atCommand.length() > 0){
//		serial.println(atCommand);
//	}
//	//Wait for the serial buffer to be emptied so we know the command has been send.
//	serial.flush();
//	const int sleepInterval = 100;
//	//Calculate the amount of tries before timeout.
//	//We try a minimum of 1 time.
//	int tries = timeout / sleepInterval + 1;
//
//	//Wait with a fixed interval for the first byte to become available.
//	for(int i=0; i<tries && serial.available() == 0; i++){
//		TaskBase::sleep(sleepInterval);
//	}
//	serial.setTimeout(sleepInterval);
//	//Read the buffer.
//	int read = serial.readBytes(buffer, length);
//	//Unlock the serial resource.
//	mutex.unlock();
//	return read;
//}
//
//bool AtClient::execute(const String &atCommand, String &data, const String &expect){
//	//Test if command begins with AT. This is a requirement for the AT protocol.
//	//If it does not begin with AT we can assume it's an invalid command.
//	if(!(	(atCommand[0] == 'A' || atCommand[0] == 'a') &&
//			(atCommand[1] == 'T' || atCommand[1] == 't')
//		)
//	){
//		return false;
//	}
//
//	//We need to put large buffers on the heap. RTOS stack is limited.
//	char *buffer = new char[bufferSize]{};
//	//Read data, but keep the last byte free to add a null terminator character.
//	int read = execute(atCommand, buffer, bufferSize - 1);
//	//Read data is not null terminated so we do it ourself.
//	buffer[read] = '\0';
//
//
//	PRINT("R: ");
//	PRINTLN(buffer);
//
//	//Parse buffer to string and remove control characters.
//	String s(buffer);
//	delete[] buffer;
//
//	//Nothing read is nothing gained, so either the command timed out or pin in not connected.
//	//Either way assume an error.
//	if(read == 0){
//		return false;
//	}
//	PRINTLN("Expected:" + expect);
//	//Test if the response contains the expected value.
//	if(s.indexOf(expect) != -1 || s.indexOf("OK") != -1){
//		PRINTLN("Expected found!");
//		//Remove the expected value from response. We know it's there so we don't need to see it anymore.
//		s = s.remove(s.lastIndexOf(expect), expect.length());
//		//Remove the leading \r\n
//		while(s[0] == '\r' || s[0] == '\n'){
//			s = s.remove(0,1);
//		}
//		//Remove the trailing \r\n
//		while(s[s.length() -1] == '\r' || s[s.length() -1] == '\n'){
//			s = s.remove(s.length() -1, 1);
//		}
//		data = s;
//		return true;
//	}
//
//	//Response did not contain the expected string.
//	return false;
//}
//
//String AtClient::scan(int timeout){
//	char *buffer = new char[bufferSize];
//	//Save original timeout.
//	int oldTimeout = getTimeout();
//	setTimeout(timeout);
//	//Read with new timeout.
//	int read = execute(String(),buffer, bufferSize - 1);
//	//Restore old timeout.
//	setTimeout(oldTimeout);
//	//Add a null terminator, this is need to convert to a String.
//	buffer[read] = '\0';
//	String s(buffer);
//	delete[] buffer;
//
//	return s;
//}
