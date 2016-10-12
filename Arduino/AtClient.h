/**
 * @file AtClient.h
 *
 * @author Nathan Schaaphuizen
 * @date 16 sep. 2016
 */

#ifndef ATCLIENT_H_
#define ATCLIENT_H_

#include <HardwareSerial.h>
/**
 * @class AtClient
 * This class lets you communicate with an AT device over a serial port.
 * The serial port is not configured and the life cycle is not managed by this class and needs to be initialized
 * before assigning it.
 */
class AtClient{
	static String voidBuffer;

private:
	HardwareSerial &serial;
	int timeout;

public:
	/** Command returned valid. **/
	static const String AT_OK;
	/** Command returned with an error. **/
	static const String AT_ERROR;

public:
	/**
	 * Create a new object.
	 * The serial port configuration will not be edited.
	 * @param serial the serial port used for communication with the AT device.
	 * @param timeout the default timeout for AT commands.
	 */
	AtClient(HardwareSerial &serial, int timeout = 500);

	/**
	 * Connect the AtClient to the AT device.
	 * Although not required, it is highly recommended to call this method before
	 * using any other utilities from this class.
	 *
	 * Note: this function will turn of the device echo.
	 * @return True if successful communication with the client happened, false if not.
	 */
	bool connect();

	/**
	 * Reset the AT device to the default user profile. Useful if you fack up.
	 * It is recommended to call connected() again after using this method.
	 * @return True if successful rested, false if not.
	 */
	bool reset();

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
	 * Execute an AT command and save the the raw response in a buffer.
	 * Read data that exceeds the buffer size will be voided.
	 * No validation will be performed.
	 * @param atCommand the full AT command.
	 * @param buffer the response buffer.
	 * @param length the length of the response buffer.
	 * @return The actual amount of bytes read.
	 */
	int execute(const String& atCommand, char* buffer, int length);

	/**
	 * Execute an AT command and parse the response as String.
	 * The response can be tested to see if it contains a expected value.
	 *
	 * The response may only exits of readable ASCII chars (e.g. value >= then 32).
	 * Any \\0 characters will be interpreted as end of string and the rest of the data
	 * will be omitted. Hence this function can not be used to read raw data.
	 * Furthermore any trailing and leading \\r and \\n characters are removed.
	 *
	 * A internal buffer of 4Kb is used to store the response. Any data stream that
	 * exceeds the buffer size will be voided.
	 *
	 * @param atCommand the full AT command.
	 * @param data a String that is filled with the AT response from the device minus the expected String.
	 * Default = data will be voided.
	 * Note: If echo mode is enable the command data send will also be in the response.
	 * @param expect a String that is expected to be in the AT response. Default = "\\r\\nOK\\r\\n"
	 * @return True if the AT response from the device contains the expected String, false if it does not.
	 * Note: if the expected string equals ERROR this function will return true even though the command may
	 * not have been executed correctly.
	 */
	bool execute(const String &atCommand, String &data = voidBuffer, const String &expect = AtClient::AT_OK);

};

#endif /* ATCLIENT_H_ */
