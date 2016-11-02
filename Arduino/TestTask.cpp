//// \cond
#include "TestTask.h"
#include <Arduino.h>
#include <stdlib.h>

#include <FreeRTOS_ARM.h>
#include "Debug.h"
#include <EEPROM.h>

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
	comTask.addCommunicationListener(*this);

	pinMode(13, OUTPUT);
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
			//PRINTLN("SPAM")
			static bool sendOnce = true;

			if(sendOnce){
				sendOnce = false;
				//unsigned char x = EEPROM.read(0);
				//x++;
				//EEPROM.write(0,x);

				//eeprom_initialize();
				//const unsigned long int *xp = 0x01;
				//int x = eeprom_read_dword(xp);
				//PRINTLN(String("EPPROM READ: ") + x);

				//comTask.send("Hallo world");
			}
			sleep(1000);
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
