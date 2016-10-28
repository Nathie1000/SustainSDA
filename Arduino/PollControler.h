/*
 * PollControler.h
 *
 *  Created on: 27 okt. 2016
 *      Author: Nathan
 */

#ifndef POLLCONTROLER_H_
#define POLLCONTROLER_H_

#include "Sensor.h"
#include "Timer.h"
#include "Flag.h"
#include "TaskBase.h"
#include "ArrayList.h"

class PollControler : public TaskBase, public TimerListener{
private:
	static PollControler *instance;

	ArrayList<Sensor*> sensors;
	Flag flag;
	Timer timer;
	PollControler();

public:
	static PollControler &getInstance();
	void run() override;
	void onTimeout(Timer &timer) override;

	void addSensor(Sensor &sensor);

};



#endif /* POLLCONTROLER_H_ */
