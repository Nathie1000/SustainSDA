//// \cond

#ifndef TESTTASK_H_
#define TESTTASK_H_

#include "TaskBase.h"
#include "Mutex.h"
#include "Flag.h"
#include "Queue.h"
#include "Timer.h"


class TestTask : public TaskBase, public TimerListener {
private:
	static Mutex mutex;
	static Mutex mutex2;
	static Flag flag;
	static Flag flag2;
	static Queue<int> queue;
	static Queue<double> queue2;

	Timer timer;

	Mutex mutex3;
	Flag flag3;
	Queue<float> queue3;

public:

	TestTask(int priority);
	void run() override;

	void onTimeout(Timer &timer) override;
};



#endif /* TESTTASK_H_ */

//// \endcond
