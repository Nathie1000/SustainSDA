/*
 * TestTask.h
 *
 *  Created on: 10 okt. 2016
 *      Author: Nathan
 */

#ifndef TESTTASK_H_
#define TESTTASK_H_

#include "TaskBase.h"

class TestTask : public TaskBase {
public:

	TestTask(int priority);
	void run() override;
};



#endif /* TESTTASK_H_ */
