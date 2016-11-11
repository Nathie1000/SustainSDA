//// \cond
#include "TestTask.h"
#include <Arduino.h>
#include <stdlib.h>

#include <FreeRTOS_ARM.h>
#include "Debug.h"
#include <EEPROM.h>

#include "ArrayList.h"

Mutex TestTask::mutex;
Mutex TestTask::mutex2;
Flag TestTask::flag;
Flag TestTask::flag2;

Queue<int> TestTask::queue(3);
Queue<double> TestTask::queue2(5);

//Timer TestTask::timer(1000);

ArrayList<String> ar;

int tel = 0;
double teld = 0.0;

TestTask::TestTask(int priority):
TaskBase(priority, "TestTask"),
queue3(5),
timer(1000),
comTask(comTask)
{
	timer.addTimerListener(*this);
	timer.start();

	pinMode(13, OUTPUT);

	ar.add("Hallo world");
	ar.add("");
	ar.add("12345");


}

void TestTask::run(){
	PRINT("TASK START: ");
	PRINTLN(getPriority())
	while(true){
		if(getPriority() == 1){
			digitalWrite(13, LOW);
			sleep(500);
			digitalWrite(13, HIGH);
			sleep(500);
		}

		else if(getPriority() == 2){
			static bool sendOnce = true;
			if(sendOnce){
				sendOnce = false;
				PRINTLN(ar.getSize());
				for(String &s : ar){
					PRINTLN(s);
				}


			}
			sleep(1000);
		}
	}
}

void TestTask::onTimeout(Timer &timer){
	//flag.set();
	//RINTLN("FLAG set from Timer.")
}


//// \endcond
