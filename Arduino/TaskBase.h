/*
 * TaskBase.h
 *
 *  Created on: 10 okt. 2016
 *      Author: Nathan
 */

#ifndef TASKBASE_H_
#define TASKBASE_H_

#include <Arduino.h>
#include "ArrayList.h"
#include <FreeRTOS_ARM.h>

class TaskBase{
private:
	static void runHelper(void *arg);
	static ArrayList<TaskBase*> tasks;

	int priority;
	String name;
	TaskHandle_t handle;

public:
	TaskBase(int priority, const String &name = "defaultTask");
	virtual ~TaskBase();

	virtual void run() = 0;

	int getPiroirty();
	String getName();

	static void startAllTasks();
	static void sleep(int time);
};


#endif /* TASKBASE_H_ */
