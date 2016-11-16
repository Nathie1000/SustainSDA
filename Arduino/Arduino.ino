#include "TestTask.h"
#include "Debug.h"

#include "CommunicationControler.h"
#include "LocationController.h"
#include "MotionControler.h"
#include "WatchDog.h"
#include "ArrayList.h"

class ComTest : public CommunicationListener, public LocationListener, public MotionListener{
public:
	ComTest(){
		CommunicationControler::getInstance().sendPostRequest("http://google.nl", "hallo wrold", this);
		//CommunicationControler::getInstance().get("http://google.nl", this);
		CommunicationControler::getInstance().sendSms("31654650997", "Hallo world!");

		LocationController::getInstance().addLocationListener(*this);
		MotionControler::getInstance().addMotionListener(*this);

	}

	void onMessageReceived(long long messageId, int responseStatus, const String &response) override{
		PRINTLN(String("id: ") + (int)messageId + " status: " + responseStatus + "message: " + response);
	}

	void onLocationFound(float latitude, float longitude) override{
		PRINTLN(String("Location: ") + latitude + ", " + longitude);
	}

	void onMotion(const MotionSensorListener::Motion &newMotion){
		PRINTLN(String("Motion: ") +newMotion.ax + ", " +newMotion.ay + ", " + newMotion.az);
	}

};

//Rules:
//Do not flush on setup.
//Tasks may not be allocated on stack.
void setup(){
	DEBUG_BEGIN(9600);

	new ComTest;
	WatchDog::getInstance().start(6000);

    TaskBase::startAllTasks();
}


void loop() {
	//not used
}
