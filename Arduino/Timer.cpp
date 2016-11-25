/**
 * @file Timer.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 22 okt. 2016
 */
#include "Timer.h"
#include <FreeRTOS_ARM.h>
#include <utility/timers.h>
#include "Debug.h"

ArrayList<Timer*> Timer::timers;

void Timer::timerCallbackFunction( TimerHandle_t xTimer ){
	for(Timer *timer : timers){
		if(timer->timer == xTimer){
			for(TimerListener *timerListener : timer->timerListeners){
				timerListener->onTimeout(*timer);
			}
			return;
		}
	}
}

Timer::Timer(int timeout):
timer(NULL)
{
	timer = xTimerCreate(NULL, pdMS_TO_TICKS(timeout), pdTRUE, NULL, timerCallbackFunction );
	if(timer == NULL){
		PRINTLN("Error creating Timer.");
	}
	else{
		timers.add(this);
	}
}

Timer::~Timer(){
	if(timer != NULL){
		if(xTimerDelete(timer, 0) != pdPASS){
			PRINTLN("Error deleting Timer.");
		}
		timers.remove(this);
	}
}

void Timer::start(){
	if(xTimerStart(timer, 0) != pdPASS){
		PRINTLN("Error starting Timer.");
	}
}

void Timer::stop(){
	if(xTimerStop(timer, 0) != pdPASS){
		PRINTLN("Error stopping Timer.")
	}
}

void Timer::addTimerListener(TimerListener &timerListener){
	timerListeners.add(&timerListener);
}

