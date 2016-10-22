/*
 * TestTask.cpp
 *
 *  Created on: 10 okt. 2016
 *      Author: Nathan
 */
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

TestTask::TestTask(int priority):
TaskBase(priority, "TestTask"),
queue3(5),
timer(1000)
{
	timer.addTimerListener(*this);
	timer.start();
}

void TestTask::run(){
	Serial3.print("TASK START: ");
	PRINTLN(getPriority())
	while(true){
		if(getPriority() == 1){
//
//
//			sleep(1);
////			TaskStatus_t satus[4];
////			TaskStatus_t status;
////			vTaskGetInfo( NULL,	status, pdTRUE, 0);
////			uxTaskGetSystemState(satus, 4, NULL);
////			PRINTLN(satus[0].usStackHighWaterMark);
////			PRINTLN(satus[1].usStackHighWaterMark);
////			PRINTLN(satus[2].usStackHighWaterMark);
////			PRINTLN(satus[3].usStackHighWaterMark);
////			sleep(1000);
//			queue.push(tel);
//			//mutex.lock();
			PRINTLN(String("TASK 1 push ") + tel);

//			PRINT("TASK 1 push ");
//			PRINTLN( tel);
//			//mutex.unlock();
			tel++;
//
////			sleep(5000);
////			PRINTLN("TAKS 1 set Flag");
////			flag.set();
//
//
//			//PRINTLN("TAKS 1 is waiting for mutex");
//			//mutex.lock();
//			//PRINTLN("TAKS 1 locked mutex");
//			//sleep(5000);
//			//PRINTLN("TAKS 1 slept for 10 sec");
//			//mutex.unlock();
//			//PRINTLN("TAKS 1 unlocked mutex");
			sleep(10);
		}

		else if(getPriority() == 2){
//			queue2.push(teld);
//			//mutex.lock();
//			//PRINTLN("TASK 2 push ");
//			//PRINTLN(teld)
//			//PRINTLN(String("TASK 2 push ") + teld);
//			//PRINTLN(String("TASK 2 push ") + teld);
//			//PRINTLN(String("TASK 2 push ") + teld);
//			//PRINTLN(String("TASK 2 push ") + teld);
//			//PRINTLN(String("TASK 2 push ") + teld);
//			//mutex.unlock();
			PRINTLN(String("TASK 2 push ") + teld);
			teld += 0.5;
//
////			sleep(1000);
////			PRINTLN("TAKS 2 set Flag2");
////			flag2.set();
//			//PRINTLN("TAKS 2 is waiting for Flag");
//			//flag.wait();
//			//PRINTLN("TAKS 2 took Flag");
//
////			PRINTLN("TAKS 2 is waiting for mutex");
////			mutex2.lock();
////			PRINTLN("TAKS 2 locked mutex");
////			sleep(5000);
////			PRINTLN("TAKS 2 slept for 1 sec");
////			mutex2.unlock();
////			PRINTLN("TAKS 2 unlocked mutex");
			sleep(10);
		}

		else if(getPriority() == 3){
//			const Waitable *w = wait(queue | queue2 | queue3 | flag);
//			if(w == &queue){
//				int i = queue.pop();
//				//PRINT("TASK 3 pop q1: ");
//				//PRINTLN(i);
//			}
//			else if(w == &queue2){
//				double d = queue2.pop();
//				//PRINT("TASK 3 pop q2: ");
//				//PRINTLN(d);
//			}
//			else if(w == &flag){
//				flag.wait();
//				//PRINTLN("TASK 3 take Flag ");
//
//			}
//			else{
//				float f = queue3.pop();
//				//PRINT("TASK 3 pop q3: ");
//				//PRINTLN(f);
//			}
//			//PRINTLN(String("Queue 1: ") + queue.peek());
//			//PRINTLN(String("Queue 2: ") + queue2.peek());
//			//sleep(100);
			sleep(10);
		}

		else{
			//PRINTLN("TASK 4 push 1.1, and also flag.");
			//queue3.push(1.1f);
			//flag.set();
			//Serial3.println(String("RUN: ") + getName() + " " + getPiroirty());
			//Serial3.flush();
			//vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);
			digitalWrite(13, LOW);
			sleep(500);
			digitalWrite(13, HIGH);
			sleep(500);

		}
	}
}

void TestTask::onTimeout(Timer &timer){
	//flag.set();
	//RINTLN("FLAG set from Timer.")
}
