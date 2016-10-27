/**
 * @file Flag.h
 *
 * @author Nathan Schaaphuizen
 * @date 14 okt. 2016
 */

#ifndef FLAG_H_
#define FLAG_H_

#include "Waitable.h"
#include <FreeRTOS_ARM.h>
#include <utility/event_groups.h>

/**
 * @class Flag
 * RTOS synchronization element.
 * A Flag can be set by one or more tasks to indicate a event.
 * Only one task can wait/take the flag.
 */
class Flag : public Waitable{
private:
	EventGroupHandle_t event;

public:
	/**
	 * Create a new object.
	 * Also registers a flag within the RTOS.
	 */
	Flag();

	/**
	 * Destroy object.
	 * Also deregisters the flag within the RTOS.
	 */
	~Flag();

	/**
	 * Set the flag.
	 * This function is non-blocking and return immediately.
	 */
	void set();

	/**
	 * Wait for the flag to be set and take it when available.
	 * This function blocks until flag is set.
	 */
	void wait();

	/**
	 * Implementation of the Waitable interface.
	 * @return True if flag is not set, otherwise false.
	 */
	bool isWaiting() const override;
};

#endif /* FLAG_H_ */
