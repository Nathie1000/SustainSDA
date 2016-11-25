/**
 * @file PollControler.h
 *
 * @author Nathan Schaaphuizen
 * @date 27 okt. 2016
*/

#ifndef POLLCONTROLER_H_
#define POLLCONTROLER_H_

#include "Sensor.h"
#include "Timer.h"
#include "Flag.h"
#include "TaskBase.h"
#include "ArrayList.h"

/**
 * @class PollControler
 * Task that periodically calls register sensor to update themselves.
 * This task uses the singleton pattern to be available when necessary.
 * The polling rate will be a predefined fixed amount. Register sensors need to decide form themselves
 * if the wish to be invoked on each event.
 */
class PollControler : public TaskBase, public TimerListener{
private:
	static PollControler *instance;
	static const int pollRate = 50;
	static const int pollPriority = 4;

	ArrayList<Sensor*> sensors;
	Flag flag;
	Timer timer;
	PollControler();

public:
	/**
	 * Get the PollControler instance. Makes one if not exits.
	 * @return reference to the PollControler instance.
	 */
	static PollControler &getInstance();

	/**
	 * Implementation of TaskBase interface.
	 */
	void run() override;

	/**
	 * Implementation of TimerListener interface.
	 * @param timer the Timer that expired.
	 */
	void onTimeout(Timer &timer) override;

	/**
	 * Add sensor to be polled.
	 * @param sensor
	 */
	void addSensor(Sensor &sensor);

	/**
	 * Get the poll rate.
	 * This is the interval between update() calls.
	 * @return the interval in ms.
	 */
	int getPollRate();

};
#endif /* POLLCONTROLER_H_ */
