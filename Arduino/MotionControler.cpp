/**
 * @file MotionControler.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 14 nov. 2016
 */

#include "MotionControler.h"
#include "TaskBase.h"
#include "Queue.h"
#include "MotionSensorClient.h"
#include "ArrayList.h"

MotionControler* MotionControler::instance = nullptr;
MotionControler & MotionControler::getInstance(){
	if(instance == nullptr){
		instance = new MotionControler();
	}
	return *instance;
}

MotionControler::MotionControler():
TaskBase(3, "MotionTask"),
motionSensor(13,14,I2C_PINS_16_17),
motionQueue(10)
{
}

void MotionControler::run(){
	PRINTLN("-----------------Motion Task Start-----------");
	if(motionSensor.openDevice()){
		motionSensor.addMotionListener(*this);
	}
	else{
		PRINTLN("No I2C device found, Motion Task suspend.");
		suspend(); // Task ends here.
	}

	while(true){
		Motion motion = motionQueue.pop();
		for(MotionListener *motionListener : motionListeners){
			motionListener->onMotion(motion);
		}
	}
}

void MotionControler::onMotion(const Motion &newMotion){
	motionQueue.push(newMotion);
}

void MotionControler::addMotionListener(MotionListener &motionListener){
	motionListeners.add(&motionListener);
}
