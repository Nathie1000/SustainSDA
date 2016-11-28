#include "SustainWork.h"
#include <ArduinoJson.h>

class ComTest : public CommunicationListener, public LocationListener, public MotionListener{
public:
	ComTest(){
		CommunicationControler::getInstance().sendPostRequest("http://www.sustain.net23.net/echo.php", "hallo world", this);
		//CommunicationControler::getInstance().get("http://google.nl", this);
		//CommunicationControler::getInstance().sendSms("31654650997", "Hallo world!");

		LocationController::getInstance().addLocationListener(*this);
		MotionControler::getInstance().addMotionListener(*this);
	}

	void onMessageReceived(long long messageId, int responseStatus, const String &response) override{
		PRINTLN(String("id: ") + (int)messageId + " status: " + responseStatus + " message: " + response);
	}

	void onLocationFound(float latitude, float longitude) override{
		PRINTLN(String("Location: ") + latitude + ", " + longitude);
	}

	void onMotion(const MotionSensorListener::Motion &newMotion){
		//PRINTLN(String("Motion: ") +newMotion.ax + ", " +newMotion.ay + ", " + newMotion.az);
		PRINTLN(String("Gyro: ") +newMotion.gx + ", " +newMotion.gy + ", " + newMotion.gz);
		//TaskBase::sleep(500);
	}
};

class TestTask : public TaskBase{
public:

	TestTask():
		TaskBase(1, "TestTask")
	{

	}

	void run() override{
		PRINTLN("-----TEST TAKS------");
		char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";

		StaticJsonBuffer<200> jsonBuffer;

		JsonObject& root = jsonBuffer.parseObject(json);

		String sensor = root["sensor"];
		String time         = root["time"];
		double latitude    = root["data"][0];
		double longitude   = root["data"][1];

		PRINTLN(sensor)
		PRINTLN(time)
		PRINTLN(latitude)
		PRINTLN(longitude);


		JsonObject& root2 = jsonBuffer.createObject();
		root2["sensor"] = "lalalala";
		root2["time"] = 1351824120;
		root2["f"] = 23.0f;

		root2.printTo(Serial);

		while(true){
			sleep(1000);
		}
	}
};

//Rules:
//Do not flush on setup.
//Tasks may not be allocated on stack.
void setup(){
	DEBUG_BEGIN(9600);

	new ComTest;
	new TestTask;

	WatchDog::getInstance().start(6000);

    TaskBase::startAllTasks();
}


void loop() {
	//not used
}
