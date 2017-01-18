/**
* @file StepDetection.h
*
* @author Bianca Krieger
* @date 9 jan. 2017
*/

#ifndef _STEPDETECTION_H
#define _STEPDETECTION_H

#include <SustainWork.h>
#include "Algorithm.h"

/**
* @class StepDetection
* @brief This class the main class of the stepdetection.
*/

class StepDetection: public MotionListener {
  public:
    uint16_t count = 0;
    ArrayList<float> inputArray;
    ArrayList<float> outputArray;
	ArrayList<float> ax, ay, az;
	ArrayList<float> mag;

	/**
	* @brief Constructor of the StepDetection class, adds an motionListener.
	*/
    StepDetection();

	/**
	* @brief onMotion Recieves the raw data of the accelerometer, gyroscope and magnetometer.
	* @param newMotion Contains the raw data of the accelerometer, gyroscope and magnetometer.
	*		 And clears the arrays after and movement has been found.
	*/
    void onMotion(const MotionSensorListener::Motion &newMotion);

};

#endif
