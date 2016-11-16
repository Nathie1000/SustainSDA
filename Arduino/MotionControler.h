/**
 * @file MotionControler.h
 *
 * @author Nathan Schaaphuizen
 * @date 14 nov. 2016
 */

#ifndef MOTIONCONTROLER_H_
#define MOTIONCONTROLER_H_

#include "TaskBase.h"
#include "Queue.h"
#include "MotionSensorClient.h"
#include "ArrayList.h"

class MotionListener{
public:
	virtual ~MotionListener(){}
	virtual void onMotion(const MotionSensorListener::Motion &newMotion) = 0;
};

class MotionControler : public TaskBase, public MotionSensorListener {
private:
	static MotionControler* instance;
	MotionSensorClient motionSensor;
	Queue<Motion> motionQueue;
	ArrayList<MotionListener*> motionListeners;

	MotionControler();
public:
	static MotionControler & getInstance();
	void run() override;
	void onMotion(const Motion &newMotion) override;
	void addMotionListener(MotionListener &motionListener);
};

#endif /* MOTIONCONTROLER_H_ */
