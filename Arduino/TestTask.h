/*
 * TestTask.h
 *
 *  Created on: 10 okt. 2016
 *      Author: Nathan
 */

#ifndef TESTTASK_H_
#define TESTTASK_H_

#include "TaskBase.h"
#include "Mutex.h"
#include "Flag.h"

class TestTask : public TaskBase {
private:
	static Mutex mutex;
	static Mutex mutex2;
	static Flag flag;
	static Flag flag2;

public:

	TestTask(int priority);
	void run() override;
};



#endif /* TESTTASK_H_ */
