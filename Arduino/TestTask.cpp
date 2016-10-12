/*
 * TestTask.cpp
 *
 *  Created on: 10 okt. 2016
 *      Author: Nathan
 */
#include "TestTask.h"
#include <Arduino.h>
#include "FreeRTOS_ARM.h"

TestTask::TestTask(int priority):
TaskBase(priority, "TestTask")
{

}

void TestTask::run(){
	Serial3.println("TASK START");
	Serial3.flush();
	while(true){
		Serial3.println(String("RUN: ") + getName() + " " + getPiroirty());
		Serial3.flush();
		vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);
	}
}
