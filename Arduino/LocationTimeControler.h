/**
 * @file LocationTimeControler
 *
 * @author Nathan Schaaphuizen
 * @date 1 nov. 2016
 */

#ifndef LOCATIONTIMECONTROLER_H_
#define LOCATIONTIMECONTROLER_H_

#include <Arduino.h>
#include "TaskBase.h"
#include "PmtkClient.h"
#include "ArrayList.h"
#include "GsmClient.h"
#include "Flag.h"
#include "Timer.h"

/**
 * @interface LocationListener
 * @brief Interface class to receive location from either the GPS or GPRS.
 */
class LocationListener{
public:
	/**
	 * Virtual deconstructor required by compiler, does nothing.
	 */
	virtual ~LocationListener(){};

	/**
	 * Prototype function triggered when a new location is available.
	 * @param latitude the latitude as a decimal point.
	 * @param longitude the longitude as a decimal point.
	 */
	virtual void onLocationFound(float latitude, float longitude) = 0;
};

/**
 * @class LocationTimeControler
 * @brief Task that handles location tracking.
 *
 * This task will will automatically keep track of the current location and time, using the GPS if available.
 * If not it will fall back on the GPRS. Keep in mind that the GPRS location may be very inaccurate and more
 * power intensive.
 * The time can is in UTC (GMT) time zone and does not compensate for daylight saving.
 */
class LocationTimeControler : public TaskBase, public TimerListener{
private:
	static LocationTimeControler * instance;
	enum State{
		USE_NONE,
		USE_GPS,
		USE_GSM
	};

	PmtkClient &pmtk;
	GsmClient &gsm;
	ArrayList<LocationListener*> locationListeners;
	Flag flag;
	float latitude;
	float longitude;
	Timer timer;

	LocationTimeControler();

public:
	 /**
	 * Get the LocationController instance.
	 * @return A reference to the LocationController that can be used.
	 */
	static LocationTimeControler & getInstance();

	/**
	 * Implementation of the BaseTask interface.
	 */
	void run() override;

	/**
	 * Implementation of the TimerListener interface.
	 * @param timer the Timer that expired.
	 */
	void onTimeout(Timer & timer) override;

	/**
	 * Get the latitude.
	 * @return the latitude as a decimal number.
	 */
	float getLatitude();

	/**
	 * Get the longitude.
	 * @return the longitude as a decimal number.
	 */
	float getLongitude();

	/**
	 * Get the second of the current minute.
	 * @return the second in range [0, 60>.
	 */
	int getSeconds();

	/**
	 * Get the minute of the current hour.
	 * @return the minute in range [0, 60>.
	 */
	int getMinutes();

	/**
	 * Get the hour of the current day.
	 * @return the hour in range [0, 24>.
	 */
	int getHours();

	/**
	 * Get the day of the current month.
	 * @return the day in range [1, 31]
	 */
	int getDay();

	/**
	 * Get the month of the current year.
	 * @return the month in range [1, 12]
	 */
	int getMonth();

	/**
	 * Get the current year.
	 * @return the year in range [0, 9999]
	 */
	int getYear();

	/**
	 * Add a LocationListener.
	 * @param locationListener the LocationListener to be added.
	 */
	void addLocationListener(LocationListener &locationListener);
};

#endif /* LOCATIONTIMECONTROLER_H_ */
