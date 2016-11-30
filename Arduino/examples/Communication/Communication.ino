/**
 * @file MotionSensor.ino
 *
 * @author Nathan Schaaphuizen
 * @date 30 nov. 2016
 */
#include <Arduino.h>
#include <SustainWork.h>

//Example class on how to get the Location data.
class CommunicationHandler : public CommunicationListener{
public:
	CommunicationHandler(){
		//The CommunicationControler can not be registered on.
		//The callback object is passed along with the request.
		//The callback object will receive exactly one callback when the message is delivered or not.
		CommunicationControler::getInstance().sendPostRequest("http://mysite.com", "HalloWorld", this);
	}

	//Callback function called by the LocationTimeControler when a new location is available.
	//Every time this function is called the Time is also synchronized with a remote device.
	void onMessageReceived(long long messageId, int responseStatus, const String &response) override{
		//200 corresponds to the HTTP 200 OK status, meaning the message has successfully been delivered.
		if(responseStatus == 200){
			PRINTLN(response);
		}
		else{
			//Status -1 corresponds to failed to send.
			PRINTLN(String("An error occurred: " + responseStatus));
		}
	}
};

void setup(){
	//Start debugging. This line will automatically be removed if debugging is disabled.
	DEBUG_BEGIN(9600);

	//Create a new instance that handles the Communication events.
	//This object is called back by the frame work an must therefore exits in dynamic memory.
	//Do not allocate anything on the stack for it will be destroyed.
	new CommunicationHandler;

	//Start the scheduler, this function does not return.
	TaskBase::startAllTasks();
}

void loop(){
	//Not used.
}
