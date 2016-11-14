/**
 * @file DateTimeControler.h
 *
 * @author Nathan Schaaphuizen
 * @date 11 nov. 2016
 */

#ifndef DATETIMECONTROLER_H_
#define DATETIMECONTROLER_H_

#include "TaskBase.h"
#include "Flag.h"
#include "Timer.h"
#include "GsmClient.h"

class DateTimeControler : public TaskBase, public TimerListener {
private:
	static DateTimeControler * insatnce;
	enum State{
			USE_NONE,
			USE_GPRS,
			USE_GPS,
		};

	State state;
	Timer syncTimer;
	Flag syncFlag;
	GsmClient &gsm;

	DateTimeControler();
public:
	static DateTimeControler & getInstance();

	void run() override;
	void onTimeout(Timer &timer) override;


	int getSeconds();
	int getMinuts();
	int getHours();

	int getDay();
	int getMonth();
	int getYear();

};

#endif /* DATETIMECONTROLER_H_ */
