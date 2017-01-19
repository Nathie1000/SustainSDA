/**
 * @file Bearer.h
 *
 * @author Nathan Schaaphuizen
 * @date 26 okt. 2016
 */

#ifndef BEARER_H_
#define BEARER_H_

#include "AtClient.h"

/**
 * @class Bearer
 * Class used to connect to a GSM network.
 */
class Bearer{
private:
	enum BearesStatus{
		CONNECTING,
		CONNECTED,
		CLOSING,
		CLOSED
	};

	int bearer;
	String apn;
	String contype;

	bool querryBearer(int &status, String &ip);
	bool openBearer();
	bool closeBearer();

protected:
	/**
	 * AtClient that can be used by inherited classes.
	 */
	AtClient &at;

public:
	/**
	 * Create new object.
	 * @param at the AtClient used for communication.
	 * @param id the Bearer id. Range [1,3].
	 * @param apn the access point name.
	 * @param contype the connection type.
	 */
	Bearer(AtClient &at, int id, const String &apn, const String &contype);

	/**
	 * Open connection to device. This is no a bearer connection just the communication
	 * between microcontroller and the bearer device.
	 * @return True if device is successfully connected, false if not.
	 */
	bool openDevice();

	/**
	 * Check if connection to device is still open. This is no a bearer connection just the communication
	 * between microcontroller and the bearer device.
	 * @return True if device is still connected, false if not.
	 */
	bool isDeviceOpen();

	/**
	 * Get the Bearer id.
	 * @return the id of the Bearer.
	 */
	int getBearerId();

	/**
	 * Get the access point name.
	 * @return the apn of the Bearer.
	 */
	String getApn();

	/**
	 * Get the connection type.
	 * @return the connection type of the Bearer.
	 */
	String getContType();

	/**
	 * Connect with the access point with the given connection type.
	 * The method can be called again if a previous connection is lost.
	 * If a connection already exits this method will do nothing.
	 * @return True if the connection succeeded, false if it didn't
	 */
	bool connect();

	/**
	 * Check if a connection to the access point exits.
	 * @return True if connected, false if not.
	 */
	bool isConnected();

	/**
	 * Close the connection.
	 * The connection can be reopened with the connect() method.
	 * @return True if the connection successfully closed, false if not.
	 */
	bool disonnect();

	/**
	 * Get local IP-address.
	 * This method only works if successfully connected.
	 * @return The IP-address as a String.
	 */
	String getIp();

};

#endif /* BEARER_H_ */
