/*
 * Mutex.h
 *
 *  Created on: 14 okt. 2016
 *      Author: Nathan
 */

#ifndef MUTEX_H_
#define MUTEX_H_

#include <FreeRTOS_ARM.h>
#include "Waitable.h"

class Mutex: public Waitable{
private:
	SemaphoreHandle_t sem;
	bool isLocked;

public:
	Mutex();
	~Mutex();

	void lock();
	void unlock();

	bool isWaiting() const override;
};



#endif /* MUTEX_H_ */
