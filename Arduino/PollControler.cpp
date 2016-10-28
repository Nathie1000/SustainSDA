/*
 * PollControler.cpp
 *
 *  Created on: 27 okt. 2016
 *      Author: Nathan
 */
#include "PollControler.h"
#include "Sensor.h"
#include "Timer.h"
#include "Flag.h"
#include "TaskBase.h"
#include "ArrayList.h"
#include "Debug.h"

PollControler *PollControler::instance = nullptr;
PollControler & PollControler::getInstance(){
	if(instance == nullptr){
		instance = new PollControler();
	}
	return *instance;
}



PollControler::PollControler():
TaskBase(4, "PollTask"),
timer(50)
{
	timer.addTimerListener(*this);
}


void PollControler::run(){
	PRINTLN("-----------------Poll Task Start-----------");

	while(true){
		flag.wait();
		for(Sensor *sensor: sensors){
			sensor->update();
		}
	}
}

void PollControler::addSensor(Sensor &sensor){
	sensors.add(&sensor);
}

void PollControler::onTimeout(Timer &timer){
	flag.set();
}
