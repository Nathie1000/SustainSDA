/**
 * @file AtClient.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 16 sep. 2016
 */

#include "AtClient.h"
#include "WProgram.h"

String AtClient::voidBuffer = "";
const String AtClient::AT_OK = "\r\nOK\r\n";
const String AtClient::AT_ERROR = "\r\nERROR\r\n";

AtClient::AtClient(HardwareSerial &serial, int timeout):
serial(serial),
timeout(timeout)
{
}

bool AtClient::connect(){
	return (execute("AT") ||  execute("AT") || execute("AT")) && execute("ATE0");
}

bool AtClient::reset(){
	return (execute("ATZ"));
}

void AtClient::setTimeout(int timeout){
	this->timeout = timeout;
}

int AtClient::getTimeout(){
	return timeout;
}

int AtClient::execute(const String& atCommand, char *buffer, int length){
	//Clear Rx buffer to remove left overs from previous serial use.
	serial.clear();
	serial.println(atCommand);
	//Wait for the serial buffer to be emptied so we know the command has been send.
	serial.flush();

	serial.setTimeout(timeout);
	int read = serial.readBytes(buffer, length);

	return read;
}


bool AtClient::execute(const String &atCommand, String &data, const String &expect){
	//Test if command begins with AT. This is a requirement for the AT protocol.
	//If it does not begin with AT we can assume it's an invalid command.
	if(!(	(atCommand[0] == 'A' || atCommand[0] == 'a') &&
			(atCommand[1] == 'T' || atCommand[1] == 't')
		)
	){
		return false;
	}

	char buffer[4096];
	//Read data, but keep the last byte free to add a null terminator character.
	int read = execute(atCommand, buffer, sizeof(buffer) - 1);
	//Read data is not null terminated so we do it ourself.
	buffer[read] = '\0';

	//Nothing read is nothing gained, so either the command timed out or pin in not connected.
	//Either way assume an error.
	if(read == 0){
		return false;
	}

	//Parse buffer to string and remove control characters.
	String s(buffer);
	//Test if the response contains the expected value.
	if(s.indexOf(expect) != -1){
		//Remove the expected value from response. We know it's there so we don't need to see it anymore.
		s = s.remove(s.lastIndexOf(expect), expect.length());
		//Remove the leading \r\n
		while(s[0] == '\r' || s[0] == '\n'){
			s = s.remove(0,1);
		}
		//Remove the trailing \r\n
		while(s[s.length() -1] == '\r' || s[s.length() -1] == '\n'){
			s = s.remove(s.length() -1, 1);
		}
		data = s;
		return true;
	}

	return false;
}
