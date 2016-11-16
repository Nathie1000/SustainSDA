/**
 * @file Mutex.h
 *
 * @author Nathan Schaaphuizen
 * @date 14 okt. 2016
 */

#ifndef MUTEX_H_
#define MUTEX_H_

#include <FreeRTOS_ARM.h>
#include "Waitable.h"

/**
 * @class Mutex
 * RTOS synchronization element.
 * A Mutex can be use by a task to lock a resource.
 * Multiple tasks can wait for the same Mutex, only one task can take
 * the Mutex at the time.
 * The Mutex uses a binary semaphore and is non-recursive and non-counting.
 */
class Mutex: public Waitable{
private:
	SemaphoreHandle_t sem;
	bool isLocked;

public:
	/**
	 * Create new object.
	 * Also register a Mutex within the RTOS.
	 */
	Mutex();

	/**
	 * Destroy the object.
	 * Also deregister the Mutex within the RTOS.
	 */
	~Mutex();

	/**
	 * Lock the Mutex.
	 * This function is blocking if the Mutex is already taken.
	 */
	void lock();

	/**
	 * Unlock the Mutex.
	 * This function returns immediately.
	 */
	void unlock();

	/**
	 * Implementation of the Waitable interface.
	 * @return True if the Mutex is not taken, otherwise false.
	 */
	bool isWaiting() const override;
};

#endif /* MUTEX_H_ */
