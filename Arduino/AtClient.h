/**
 * @file AtClient.h
 *
 * @author Nathan Schaaphuizen
 * @date 16 sep. 2016
 */

#ifndef ATCLIENT_H_
#define ATCLIENT_H_

#include <HardwareSerial.h>
#include "Mutex.h"

/**
 * @class AtClient
 * @brief This class lets you communicate with an AT device over a serial port.
 *
 * The serial life cycle is managed by this class and should not be used by any other instances.
 */
class AtClient{

private:
	static AtClient *instance;
	static String voidBuffer;
	static const int bufferSize = 1024;

	HardwareSerial &serial;
	int timeout;
	Mutex mutex;

	/**
	 * Create a new object.
	 * The serial port will be initialized with the given baud rate and should not be used form now on.
	 * @param serial an un-initialized serial port used for communication with the AT device.
	 * @param baudrate the baud rate used to initialize the serial port.
	 * @param timeout the default timeout for AT commands.
	 */
	AtClient(HardwareSerial &serial, int baudrate, int timeout = 500);

public:
	 /**
	 * Get the AtClient instance.
	 * @return A reference to the AtClient that can be used.
	 */
	static AtClient & getInstance();

	/** Command returned valid. **/
	static const String AT_OK;
	/** Command returned with an error. **/
	static const String AT_ERROR;

	/**
	 * Destroy the object.
	 * The serial port is closed and can be used from now on.
	 */
	~AtClient();

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
	 * Test if the AtClient is connected. That is to say: test if the AT device is still
	 * responsive the the AT command.
	 * @return True if AT device is still connected, false if not.
	 */
	bool isConnected();

	/**
	 * Reset the AT device to the default user profile. Useful if you fack up.
	 * It is recommended to call connected() again after using this method.
	 * @return True if successful rested, false if not.
	 */
	bool reset();

	/**
	 * Send a command to the AT device.
	 * @param atCommand the full AT command.
	 */
	void send(const String& atCommand);

	/**
	 * Scan for response from the AT device for x milliseconds.
	 * @param timeout the amount of time to wait in milliseconds before returning the function.
	 * @return A string containing all data the the AT device send between this call and timeout.
	 */
	String scan(int timeout = 200);

	/**
	 * Scan for specific response from the AT device.
	 * This function will return when either the requested String if found or a timeout occurred.
	 * @param until a String that is matched to the AT response.
	 * @param timeout the amount of time to wait in milliseconds before returning the function.
	 * @return A string containing all data the the AT device send between this call and timeout/until.
	 */
	String scan(const String& until, int timeout = 200);

	/**
	 * Check if response is valid (contains OK).
	 * If so remove the OK from string.
	 * @param response the response from the AT device to check.
	 * @return True if response is valid, false if not.
	 */
	bool isOk(String& response);

	/**
	 * Check if response is invalid (contains ERROR).
	 * If so remove the ERROR from string.
	 * @param response the response from the AT device to check.
	 * @return True if response is an error, false if not.
	 */
	bool isError(String& response);


	/**
	 * Set timeout. This is the time maximum waited for a response from the device.
	 * @param timeout the timeout time in ms.
	 */
	//void setTimeout(int timeout);

	/**
	 * Get timeout. This is the time maximum waited for a response from the device.
	 * @return The timeout time in ms.
	 */
	//int getTimeout();

	/**
	 * Execute an AT command and save the the raw response in a buffer.
	 * Read data that exceeds the buffer size will be voided.
	 * No validation will be performed.
	 * @param atCommand the full AT command.
	 * @param buffer the response buffer.
	 * @param length the length of the response buffer.
	 * @return The actual amount of bytes read.
	 */
	//int execute(const String& atCommand, char* buffer, int length);

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
	//bool execute(const String &atCommand, String &data = voidBuffer, const String &expect = AtClient::AT_OK);

	/**
	 * Scan the bus for any response from the device.
	 * This function will block until any data if found or the timeout is reached.
	 * @param timeout the amount of time in ms to wait.
	 * @return the response parsed as a String.
	 */
	//String scan(int timeout);
};

#endif /* ATCLIENT_H_ */
