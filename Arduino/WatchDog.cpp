/**
 * @file WatchDog.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 31 okt. 2016
 */
#include "WatchDog.h"
#include "Debug.h"

IntervalTimer WatchDog::timer;

#define CPU_RESTART_ADDR (uint32_t *)0xE000ED0C
#define CPU_RESTART_VAL 0x5FA0004
#define CPU_RESTART (*CPU_RESTART_ADDR = CPU_RESTART_VAL);

void WatchDog::restart(){
	PRINTLN("WatchDog: arf, arf");
	PRINTLN("The system did not respond. Rebooting now!")
	delay(500);
	CPU_RESTART;
}

WatchDog::WatchDog(int timeout):
TaskBase(4, "WatchDogTask"),
timeout(timeout)
{
	timer.begin(restart, timeout*1000);
}

void WatchDog::run(){
	PRINTLN("-----------------WatchDog Task Start-----------");
	timer.end();
	while(true){
		timer.begin(restart,  timeout*1000);
		//We sleep 90% of the waited time.
		int sleepTime = (int)(timeout * 0.90f);
		sleep(sleepTime);
		timer.end();
	}
}
