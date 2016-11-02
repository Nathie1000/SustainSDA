/**
 * @file TaskBase.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 10 okt. 2016
 */

#include "TaskBase.h"
#include <Arduino.h>
#include <FreeRTOS_ARM.h>
#include "Debug.h"
#include "Waitable.h"
#include "CompositeWaitable.h"


void TaskBase::runHelper(void *arg){
#ifdef DEBUG
	sleep(1000);
#endif
	TaskBase *task = (TaskBase*)arg;
    task->run();

	PRINTLN("Warning: Task may not end! Task suspended!");
	vTaskSuspend(task->handle);
	//taskYIELD();
}

ArrayList<TaskBase*> TaskBase::tasks(5);

TaskBase::TaskBase(int priority, const String &name):
priority(priority),
name(name),
handle(nullptr)
{
	tasks.add(this);
}

TaskBase::~TaskBase(){
	tasks.remove(this);
	if(handle != nullptr){
		vTaskDelete(handle);
	}
}

int TaskBase::getPriority(){
	return priority;
}

String TaskBase::getName(){
	return name;
}

void TaskBase::startAllTasks(){
	for(TaskBase *task : tasks){
		xTaskCreate(runHelper, task->getName().c_str(), taskSize, task, task->getPriority(), &task->handle);
	}
	vTaskStartScheduler();
	PRINTLN("Task scheduling failed: Insufficient RAM");
}

void TaskBase::sleep(int time){
	vTaskDelay((time * configTICK_RATE_HZ) / 1000L);
}

const Waitable* TaskBase::wait(const CompositeWaitable &compositeWaitable){
	while(true){
		for(const Waitable *waitable : compositeWaitable.getWaitables()){
			if(!waitable->isWaiting()){
				return waitable;
			}
		}
		TaskBase::sleep(1);
	}
}

const Waitable* TaskBase::wait(const Waitable &waitable){
	while(true){
		if(!waitable.isWaiting()){
			return &waitable;
		}
		TaskBase::sleep(1);
	}
}

//void TaskBase::yield(){
//	taskYIELD();
//}

