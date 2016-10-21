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
#include <stdlib.h>

Mutex TestTask::mutex;
Mutex TestTask::mutex2;
Flag TestTask::flag;
Flag TestTask::flag2;

Queue<int> TestTask::queue(3);
Queue<double> TestTask::queue2(5);

int tel = 0;
double teld = 0.0;

TestTask::TestTask(int priority):
TaskBase(priority, "TestTask"),
queue3(5)
{

}

void TestTask::run(){
	Serial3.println("TASK START");
	Serial3.flush();
	while(true){
		if(getPriority() == 1){
			queue.push(tel);
			//mutex.lock();
			PRINTLN(String("TASK 1 push ") + tel);
			//mutex.unlock();
			tel++;

//			sleep(5000);
//			PRINTLN("TAKS 1 set Flag");
//			flag.set();


			//PRINTLN("TAKS 1 is waiting for mutex");
			//mutex.lock();
			//PRINTLN("TAKS 1 locked mutex");
			//sleep(5000);
			//PRINTLN("TAKS 1 slept for 10 sec");
			//mutex.unlock();
			//PRINTLN("TAKS 1 unlocked mutex");
			sleep(1000);
		}

		else if(getPriority() == 2){
			queue2.push(teld);
			//mutex.lock();
			PRINTLN(String("TASK 2 push ") + teld);
			//mutex.unlock();
			teld += 0.5;

//			sleep(1000);
//			PRINTLN("TAKS 2 set Flag2");
//			flag2.set();
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
			sleep(2000);
		}

		else if(getPriority() == 3){
			const Waitable *w = wait(queue | queue2 | queue3 | flag);
			if(w == &queue){
				int i = queue.pop();
				PRINTLN(String("TASK 3 pop q1: ") + i);
			}
			else if(w == &queue2){
				double d = queue2.pop();
				PRINTLN(String("TASK 3 pop q2: ") + d);
			}
			else if(w == &flag){
				flag.wait();
				PRINTLN(String("TASK 3 take Flag "));
			}
			else{
				float f = queue3.pop();
				PRINTLN(String("TASK 3 pop q3: ") + f);
			}
			//PRINTLN(String("Queue 1: ") + queue.peek());
			//PRINTLN(String("Queue 2: ") + queue2.peek());
			//sleep(100);
		}

		else{
			PRINTLN("TASK 4 push 1.1, and also flag.");
			queue3.push(1.1f);
			flag.set();
			//Serial3.println(String("RUN: ") + getName() + " " + getPiroirty());
			//Serial3.flush();
			vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);
		}
	}
}
