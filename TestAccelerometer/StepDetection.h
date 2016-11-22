#include <SustainWork.h>
#include "Algorithm.h"

class StepDetection: public MotionListener {
  public:
    uint16_t count = 0;
    ArrayList<float> inputArray;
    ArrayList<float> outputArray;
	ArrayList<float> ax, ay, az;

    StepDetection();
    void onMotion(const MotionSensorListener::Motion &newMotion);

};
