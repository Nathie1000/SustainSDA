/*
 * Timer.h
 *
 *  Created on: 22 okt. 2016
 *      Author: Nathan
 */

#ifndef TIMER_H_
#define TIMER_H_
#include "ArrayList.h"
#include <FreeRTOS_ARM.h>
#include <utility/timers.h>

class Timer;

class TimerListener{
public:
	virtual ~TimerListener(){};
	virtual void onTimeout(Timer &timer) = 0;
};

class Timer{
private:
	static ArrayList<Timer*> timers;
	static void timerCallbackFunction( TimerHandle_t xTimer );

	ArrayList<TimerListener*> timerListeners;
	TimerHandle_t timer;

public:
	explicit Timer(int timeout);
	~Timer();
	void start();
	void stop();

	void addTimerListener(TimerListener &timerListener);

};



#endif /* TIMER_H_ */
