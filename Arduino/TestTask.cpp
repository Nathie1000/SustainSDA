/*
 * TestTask.cpp
 *
 *  Created on: 10 okt. 2016
 *      Author: Nathan
 */
#include "TestTask.h"
#include <Arduino.h>
#include "FreeRTOS_ARM.h"
#include "Debug.h"

Mutex TestTask::mutex;
Mutex TestTask::mutex2;
Flag TestTask::flag;
Flag TestTask::flag2;

TestTask::TestTask(int priority):
TaskBase(priority, "TestTask")
{

}

void TestTask::run(){
	Serial3.println("TASK START");
	Serial3.flush();
	while(true){
		if(getPiroirty() == 1){
			sleep(5000);
			PRINTLN("TAKS 1 set Flag");
			flag.set();


			//PRINTLN("TAKS 1 is waiting for mutex");
			//mutex.lock();
			//PRINTLN("TAKS 1 locked mutex");
			///sleep(5000);
			//PRINTLN("TAKS 1 slept for 10 sec");
			//mutex.unlock();
			//PRINTLN("TAKS 1 unlocked mutex");
			//sleep(100);
		}

		else if(getPiroirty() == 2){
			sleep(1000);
			PRINTLN("TAKS 2 set Flag2");
			flag2.set();
			//PRINTLN("TAKS 2 is waiting for Flag");
			//flag.wait();
			//PRINTLN("TAKS 2 took Flag");

//			PRINTLN("TAKS 2 is waiting for mutex");
//			mutex2.lock();
//			PRINTLN("TAKS 2 locked mutex");
//			sleep(5000);
//			PRINTLN("TAKS 2 slept for 1 sec");
//			mutex2.unlock();
//			PRINTLN("TAKS 2 unlocked mutex");
			//sleep(300);
		}

		else if(getPiroirty() == 3){
			const Waitable *w = wait(flag | flag2);

			if(w == &flag){
				flag.wait();
				PRINTLN("TAKSK 3 woke on FLAG");
			}
			else{
				flag2.wait();
				PRINTLN("TAKSK 3 woke on FLAG 2");
			}
			//sleep(100);

		}

		else{
			//Serial3.println(String("RUN: ") + getName() + " " + getPiroirty());
			//Serial3.flush();
			vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);
		}
	}
}
