/**
 * @file PmtkClient.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 1 nov. 2016
 */

#include "PmtkClient.h"
#include "Debug.h"
#include "TaskBase.h"

PmtkClient::PmtkClient(HardwareSerial &serial, int timeout):
serial(serial),
timeout(timeout)
{
	serial.begin(9600);
}

PmtkClient::~PmtkClient(){
	serial.end();
}

void PmtkClient::setTimeout(int timeout){
	this->timeout = timeout;
}

int PmtkClient::getTimeout(){
	return timeout;
}

bool PmtkClient::execute(const String &cmd){
	serial.clear();

	if(cmd[0] != '$'){
		return false;
	}

	String checksum = getChecksum(cmd.substring(1));
	String pmtkPacket = cmd + checksum + "\r\n";

	PRINTLN(pmtkPacket);

	serial.print(pmtkPacket);
	serial.flush();

	const int sleepInterval = 10;
	//Calculate the amount of tries before timeout.
	//We try a minimum of 1 time.
	int tries = timeout / sleepInterval + 1;

	//Wait with a fixed interval for the first byte to become available.
	for(int i=0; i<tries && serial.available() == 0; i++){
		TaskBase::sleep(sleepInterval);
	}
	serial.setTimeout(sleepInterval);
	//Read the buffer.
	//int read = serial.readBytes(buffer, length);

	String response = serial.readString();
	PRINTLN(response);

	return true;

}

String PmtkClient::getChecksum(const String &s){
	int sum = s[0];
	for(int i=1; i<s.length(); i++){
		sum ^= s[i];
	}
	return String(sum, HEX);
}
