/*
 * GsmClient.h
 *
 *  Created on: 26 okt. 2016
 *      Author: Nathan
 */

#ifndef GSMCLIENT_H_
#define GSMCLIENT_H_

#include "Bearer.h"
#include "ArrayList.h"
#include "Sensor.h"

class GsmListener{
public:
	virtual ~GsmListener(){}
	virtual void onLocationAndDateTimeFound(float latitude, float longitude, const String &date , const String &time) = 0;
};


/**
 * @class GsmClient
 * Class used for GSM communication.
 */
class GsmClient : public Bearer, public Sensor{
private:
	static GsmClient * instance;
	ArrayList<GsmListener*> gsmListeners;
	int lastPollTime;

	/**
	 * Create a new object.
	 * @param at the AtClient used for communication.
	 */
	explicit GsmClient(AtClient &at);

public:
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

	bool sendSms(const String &number, const String text);

	void addGsmListener(GsmListener &gsmListener);

	void update() override;

};

#endif /* GSMCLIENT_H_ */
