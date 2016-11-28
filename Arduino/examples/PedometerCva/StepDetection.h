#include <SustainWork.h>
#include "Algorithm.h"

class StepDetection: public MotionListener {
private:
    int stepCount;
    int lastTime;

    ArrayList<float> inputArray;
    ArrayList<float> outputArray;
	ArrayList<float> ax, ay, az;
	ArrayList<float> mag;

	/**
	 * Check if a amount of time has passed.
	 * @param interval time passed in seconds.
	 * @return True if interval has passed since last call to this function. False if not.
	 */
	bool isTimeToSend(int interval);
	void sendSteps();

public:

    StepDetection();
    void onMotion(const MotionSensorListener::Motion &newMotion) override;


};
