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

/**
 * @interface MotionListener
 * @brief Interface class to receive motion events.
 */
class MotionListener{
public:
	/**
	 * Virtual deconstructor required by compiler, does nothing.
	 */
	virtual ~MotionListener(){}

	/**
	 * Prototype function triggered when a new motion point is available.
	 * @param newMotion struct containing the motion data of a single point.
	 */
	virtual void onMotion(const MotionSensorListener::Motion &newMotion) = 0;
};

/**
 * @class MotionControler
 * @brief Task that handles all motion detection.
 *
 * The data from the MotionSensor will be queued and dispatched to the listeners.
 * No extra filtering is applied.
 */
class MotionControler : public TaskBase, public MotionSensorListener {
private:
	static MotionControler* instance;
	MotionSensorClient motionSensor;
	Queue<Motion> motionQueue;
	ArrayList<MotionListener*> motionListeners;

	MotionControler();
public:
	/**
	 * Get the MotionControler instance.
	 * @return A reference to the MotionControler that can be used.
	 */
	static MotionControler & getInstance();

	/**
	 * Implementation of the BaseTask interface.
	 */
	void run() override;

	/**
	 * Implementation of the MotionSensorListener interface.
	 * @param newMotion the new motion point.
	 */
	void onMotion(const Motion &newMotion) override;

	/**
	 * Add a MotionListener.
	 * @param motionListener the MotionListener to be added.
	 */
	void addMotionListener(MotionListener &motionListener);
};

#endif /* MOTIONCONTROLER_H_ */
