/**
 * @file GsmClient.h
 *
 * @author Nathan Schaaphuizen
 * @date 26 okt. 2016
 */

#ifndef GSMCLIENT_H_
#define GSMCLIENT_H_

#include "Bearer.h"

/**
 * @class GsmClient
 * Class used for GSM communication.
 */
class GsmClient : public Bearer{
private:
	static GsmClient * instance;

	/**
	 * Create a new object.
	 * @param at the AtClient used for communication.
	 */
	explicit GsmClient(AtClient &at);

public:
	/**
	 * Get the GsmClient instance.
	 * @return A reference to the GsmClient that can be used.
	 */
	static GsmClient & getInstance();

	/**
	 * States the PIN can be in.
	 */
	enum PinState{
		ERROR,     //!< ERROR
		READY,     //!< READY
		SIM_PIN,   //!< SIM_PIN
		SIM_PUK,   //!< SIM_PUK
		PH_SIM_PIN,//!< PH_SIM_PIN
		PH_SIM_PUK,//!< PH_SIM_PUK
	};

	/**
	 * Set pin code.
	 * @param pin the pin code.
	 * @return True if command executed successfully, otherwise false.
	 */
	bool setPinCode(const String &pin);

	/**
	 * Get the pin state.
	 * @return the pin state. See enum PinState.
	 */
	PinState getPinState();

	/**
	 * Get the signal quality.
	 * @return The signal quality ranging from 0 to 99 or -1 on error.
	 */
	int getSignalQuality();

	/**
	 * Get the location and time from the GSM network.
	 * The timezone is GTM by default and does not account for DLS.
	 * The latitude and longitude have a accuracy fault of += 100 meter.
	 * @param latitude the latitude.
	 * @param longitude the longitude.
	 * @param date a date String formated as yyyy/mm/dd
	 * @param time a time String formated as hh:mm:ss
	 * @return True if command executed successfully, otherwise false.
	 */
	bool getLocationAndTime(float &latitude, float &longitude, String &date, String &time);

	/**
	 * Send a SMS.
	 * @param number the phone number to send the SMS to. Must be complete number include the country prefix.
	 * e.g. +31654650997
	 * @param text the SMS body.
	 */
	bool sendSms(const String &number, const String text);
};

#endif /* GSMCLIENT_H_ */
