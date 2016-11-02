/**
 * @file PmtkClient.h
 *
 * @author Nathan Schaaphuizen
 * @date 1 nov. 2016
 */

#ifndef PMTKCLIENT_H_
#define PMTKCLIENT_H_

#include "HardwareSerial.h"

class PmtkClient {
private:
	HardwareSerial &serial;

	String getChecksum(const String &s);
	int timeout;

public:
	PmtkClient(HardwareSerial &serial, int timeout = 500);
	~PmtkClient();

	void setTimeout(int timeout);
	int getTimeout();

	bool execute(const String &cmd);

};

#endif /* PMTKCLIENT_H_ */
