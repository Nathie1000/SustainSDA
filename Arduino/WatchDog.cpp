/**
 * @file WatchDog.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 31 okt. 2016
 */
#include "WatchDog.h"
#include "Debug.h"

WatchDog* WatchDog::instance = nullptr;
IntervalTimer WatchDog::timer;

/// \cond
#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);
/// \endcond

void WatchDog::restart(){
	PRINTLN("WatchDog: arf, arf");
	PRINTLN("The system did not respond. Rebooting now!")
	delay(500);
	CPU_RESTART;
}

WatchDog & WatchDog::getInstance(){
	if(instance == nullptr){
		instance = new WatchDog();
	}
	return *instance;
}

WatchDog::WatchDog():
TaskBase(4, "WatchDogTask"),
timeout(-1)
{
}

void WatchDog::run(){
	PRINTLN("-----------------WatchDog Task Start-----------");
	while(true){
		if(timeout > 0){
			timer.begin(restart,  timeout*1000);
			//We sleep 90% of the waited time.
			int sleepTime = (int)(timeout * 0.90f);
			sleep(sleepTime);
			timer.end();
		}
		else{
			sleep(1000);
		}
	}
}

void WatchDog::start(int timeout){
	this->timeout = timeout;
}
