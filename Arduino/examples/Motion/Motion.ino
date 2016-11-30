/**
 * @file MotionSensor.ino
 *
 * @author Nathan Schaaphuizen
 * @date 30 nov. 2016
 */
#include <Arduino.h>
#include <SustainWork.h>

//Example class on how to get the Motion data.
class MotionHandler : public MotionListener{
public:
	MotionHandler(){
		//Register this object with the Motion Controller.
		MotionControler::getInstance().addMotionListener(*this);
	}

	//Callback function called by the MotionControler everything new data is available.
	void onMotion(const Motion &newMotion) override{
		PRINTLN(String("x: ") + newMotion.ax + " y:" + newMotion.ay + " z:" + newMotion.az);
	}
};


void setup(){
	//Start debugging. This line will automatically be removed if debugging is disabled.
	DEBUG_BEGIN(9600);

	//Create a new instance that handles the Motion events.
	//This object is called back by the frame work an must therefore exits in dynamic memory.
	//Do not allocate anything on the stack for it will be destroyed.
	new MotionHandler;

	//Start the scheduler, this function does not return.
	TaskBase::startAllTasks();
}

void loop(){
	//Not used.
}



