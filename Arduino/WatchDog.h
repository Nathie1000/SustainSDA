/**
 * @file WatchDog.h
 *
 * @author Nathan Schaaphuizen
 * @date 31 okt. 2016
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include "TaskBase.h"

/**
 * @class WatchDog
 * A task that checks the global deadline of all other tasks.
 * It does this by setting a hardware timer with a interrupt service routine.
 * The timer gets reset everything this task receives processing time.
 * This task has the highest priority and should thus always receive processing time unless a other
 * task starves the system.
 * This task does not detect starvation between two other tasks.
 * If a timeout occurs this task will restart the system.
 * This task can be used to detect if the system at any point for what ever reason took longer then expected.
 * It can not be used to monitor individual tasks.
 */
class WatchDog : public TaskBase{
private:
	static void restart();
	static IntervalTimer timer;

	int timeout;

public:
	/**
	 * Create a new object.
	 * @param timeout the global system timeout in ms.
	 */
	WatchDog(int timeout);

	/**
	 * Implementation of the TaskBase interface.
	 */
	void run() override;
};

#endif /* WATCHDOG_H_ */
