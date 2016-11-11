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
	static PmtkClient *instance;
	HardwareSerial &serial;

	String getChecksum(const String &s);
	int timeout;

	PmtkClient(HardwareSerial &serial, int baudrate, int timeout = 500);

public:
	static PmtkClient & getInstance();

	~PmtkClient();

	void setTimeout(int timeout);
	int getTimeout();

	bool execute(const String &cmd);

};

#endif /* PMTKCLIENT_H_ */
