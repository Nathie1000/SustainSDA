//// \cond
#include "TestTask.h"
#include <Arduino.h>
#include <stdlib.h>

#include "FreeRTOS_ARM.h"
#include "Debug.h"


Mutex TestTask::mutex;
Mutex TestTask::mutex2;
Flag TestTask::flag;
Flag TestTask::flag2;

Queue<int> TestTask::queue(3);
Queue<double> TestTask::queue2(5);

//Timer TestTask::timer(1000);

int tel = 0;
double teld = 0.0;

TestTask::TestTask(int priority, CommunicationControler &comTask):
TaskBase(priority, "TestTask"),
queue3(5),
timer(1000),
comTask(comTask)
{
	timer.addTimerListener(*this);
	timer.start();
	comTask.addCommunicationTaskListener(*this);
}

void TestTask::run(){
	Serial3.print("TASK START: ");
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
				comTask.send("Hallo world");
			}
			sleep(1000);
		}

		else if(getPriority() == 3){
			sleep(10);
		}
	}
}

void TestTask::onTimeout(Timer &timer){
	//flag.set();
	//RINTLN("FLAG set from Timer.")
}

void TestTask::onMessageReceived(const String& msg){
	PRINTLN("MESSAGE: " + msg);
}

//// \endcond
