/**
 * @file Timer.h
 *
 * @author Nathan Schaaphuizen
 * @date 22 okt. 2016
 */

#ifndef TIMER_H_
#define TIMER_H_
#include "ArrayList.h"
#include <FreeRTOS_ARM.h>
#include <utility/timers.h>

class Timer;

/**
 * @interface TimerListener
 * Callback interface.
 */
class TimerListener{
public:
	/**
	 * Virtual deconstructor required by compiler, does nothing.
	 */
	virtual ~TimerListener(){};

	/**
	 * Prototype function for the timer callback.
	 * This function is called from the TimerTask, no heavy computing should be done on it.
	 * @param timer the Timer that called.
	 */
	virtual void onTimeout(Timer &timer) = 0;
};

/**
 * @class Timer
 * RTOS synchronization element.
 * A Timer can be used to periodically notify a task.
 * A task can have multiple timers and a timer can have multiple tasks.
 */
class Timer{
private:
	static ArrayList<Timer*> timers;
	static void timerCallbackFunction( TimerHandle_t xTimer );

	ArrayList<TimerListener*> timerListeners;
	TimerHandle_t timer;

public:
	/**
	 * Create a new object.
	 * Also registers a Timer within the RTOS.
	 * @param timeout the interval in ms on which the timer triggers.
	 */
	explicit Timer(int timeout);

	/**
	 * Destroy the object.
	 * Also deregisters the Timer within the RTOS.
	 */
	~Timer();

	/**
	 * Start the timer.
	 * This function must be called before the timer does anything.
	 */
	void start();

	/**
	 * Stop the timer.
	 * The timer can be restarted with the start() function.
	 */
	void stop();

	/**
	 * Add a TimerListener.
	 * This is the object(s) that receive a notification that the timer has passed.
	 * @param timerListener the TimerListener to be added.
	 */
	void addTimerListener(TimerListener &timerListener);
};

#endif /* TIMER_H_ */
