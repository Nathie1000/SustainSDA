/*
 * GsmClient.h
 *
 *  Created on: 26 okt. 2016
 *      Author: Nathan
 */

#ifndef GSMCLIENT_H_
#define GSMCLIENT_H_

#include "Bearer.h"

class GsmClient : public Bearer{

public:
	enum PinState{
		ERROR,
		READY,
		SIM_PIN,
		SIM_PUK,
		PH_SIM_PIN,
		PH_SIM_PUK,
	};

	explicit GsmClient(AtClient &at);

	bool setPinCode(const String &pin);
	PinState getPinState();
	int getSignalQuality();

	bool getLocationAndTime(float &latitude, float &longitude, String &date, String &time);

};



#endif /* GSMCLIENT_H_ */
