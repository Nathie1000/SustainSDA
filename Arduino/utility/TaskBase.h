/**
 * @file TaskBase.h
 *
 * @author Nathan Schaaphuizen
 * @date 10 okt. 2016
 */

#ifndef TASKBASE_H_
#define TASKBASE_H_

#include <Arduino.h>
#include "ArrayList.h"
#include <FreeRTOS_ARM.h>
#include "Waitable.h"
#include "CompositeWaitable.h"

/**
 * @interface TaskBase
 * Abstract base class that defines basic task functionality.
 * A task must implement this class.
 */
class TaskBase{
public:
	/**
	 * Stack size in WORDS.
	 */
	static const unsigned short taskSize = 1024;

private:
	static void runHelper(void *arg);
	static ArrayList<TaskBase*> tasks;

	int priority;
	String name;
	TaskHandle_t handle;

public:
	/**
	 * Create a new object.
	 * Also registers a Task within RTOS.
	 * @param priority the task priority. Higher number means higher priority.
	 * @param name the task name. Mainly used for debugging.
	 */
	TaskBase(int priority, const String &name = "defaultTask");

	/**
	 * Destroy the object
	 * Also deregisters the Task within RTOS.
	 */
	virtual ~TaskBase();

	/**
	 * Prototype function that declare the entry point for a task.
	 * Heavy computing should be done within this function.
	 * This function should not return. If it does the Task will be suspend.
	 */
	virtual void run() = 0;

	/**
	 * Get the Task priority.
	 * @return the priority of the Task.
	 */
	int getPriority();

	/**
	 * Get the Task name.
	 * @return the name of the Task.
	 */
	String getName();

	/**
	 * Suspend the Task.
	 * Suspended Tasks will not no receive any CPU time;
	 */
	void suspend();

	/**
	 * Create all tasks an start the scheduler.
	 * This scheduler has control from this point on.
	 * This function only returns if the scheduler failed to start.
	 */
	static void startAllTasks();

	/**
	 * Yield the current Task for a fixed amount of time.
	 * @param time the time in ms that the Task should sleep.
	 */
	static void sleep(int time);

	/**
	 * Wait for any of the Waitables in the CompositeWaitable to become available.
	 * This function does not change the state of any Waitable in any way.
	 *
	 * The Waitables are checked in order of addition. To avoid starvation Waitables
	 * should be added in reverse priority order.
	 * e.g. the lowest priority Waitable should be added first and the highest priority as last.
	 * @param compositeWaitable the CompositeWaitable holding all the Waitables.
	 * @return A pointer to the first Waitable that becomes available.
	 */
	static const Waitable* wait(const CompositeWaitable &compositeWaitable);

	/**
	 * Wait for a single Waitable.
	 * This function does not change the state of any Waitable in any way.
	 * This function is debatable redundant but is added for completeness anyway.
	 * @param Waitable the Waitable to wait for.
	 * @return A pointer to the Waitable.
	 */
	static const Waitable* wait(const Waitable &Waitable);
};

#endif /* TASKBASE_H_ */
