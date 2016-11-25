/**
 * @file PmtkClient.h
 *
 * @author Nathan Schaaphuizen
 * @date 1 nov. 2016
 */

#ifndef PMTKCLIENT_H_
#define PMTKCLIENT_H_

#include "HardwareSerial.h"

/**
 * @class PmtkClient
 * @brief This class lets you communicate with an PMTK device over a serial port.
 *
 * The serial life cycle is managed by this class and should not be used by any other instances.
 */
class PmtkClient {
private:
	static PmtkClient *instance;
	HardwareSerial &serial;

	String getChecksum(const String &s);
	int timeout;

	/**
	 * Create a new object.
	 * The serial port will be initialized with the given baud rate and should not be used form now on.
	 * @param serial an un-initialized serial port used for communication with the PMTK device.
	 * @param baudrate the baud rate used to initialize the serial port.
	 * @param timeout the default timeout for AT commands.
	 */
	PmtkClient(HardwareSerial &serial, int baudrate, int timeout = 500);

public:
	 /**
	 * Get the PmtkClient instance.
	 * @return A reference to the PmtkClient that can be used.
	 */
	static PmtkClient & getInstance();

	/**
	 * Destroy the object.
	 * The serial port is closed and can be used from now on.
	 */
	~PmtkClient();

	/**
	 * Set timeout. This is the time maximum waited for a response from the device.
	 * @param timeout the timeout time in ms.
	 */
	void setTimeout(int timeout);

	/**
	 * Get timeout. This is the time maximum waited for a response from the device.
	 * @return The timeout time in ms.
	 */
	int getTimeout();

	/**
	 * Execute PMTK command.
	 * Function is still WIP.
	 * @param cmd the PMTK command to be executed.
	 * @return True if command executed successfully, false if not.
	 */
	bool execute(const String &cmd);

};

#endif /* PMTKCLIENT_H_ */
