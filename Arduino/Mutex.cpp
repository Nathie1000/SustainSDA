/**
 * @file Mutex.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 14 okt. 2016
 */
#include "Mutex.h"
#include "Debug.h"

Mutex::Mutex():
sem(NULL),
isLocked(false)
{
	sem = xSemaphoreCreateBinary();
	if(sem == NULL){
		PRINTLN("Error creating Mutex.")
	}
	else{
		unlock();
	}
}

Mutex::~Mutex(){
	if(sem != NULL){
		//vSemaphoreDelete(sem);
	}
}

void Mutex::lock(){
	if(xSemaphoreTake(sem, portMAX_DELAY) != pdPASS){
		PRINTLN("Error while locking Mutex.");
	}
	else{
		isLocked = true;
	}
}

void Mutex::unlock(){
	if(xSemaphoreGive(sem) != pdPASS){
		PRINTLN("Error while unlocking Mutex.");
	}
	else{
		isLocked = false;
	}
}

bool Mutex::isWaiting() const{
	return isLocked;
}


