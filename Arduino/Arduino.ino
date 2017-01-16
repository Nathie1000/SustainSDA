#include "SustainWork.h"
#include <ArduinoJson.h>

class ComTest : public CommunicationListener, public LocationListener, public MotionListener{
public:
	ComTest(){
		//CommunicationControler::getInstance().enableEncryption("thisiskey");
		//CommunicationControler::getInstance().sendPostRequest("http://www.sustain.net23.net/echo.php", "ok nu toaal iets ander met ? hallo world met meer text dat dus langer is", this);
		//CommunicationControler::getInstance().get("http://google.nl", this);
		//CommunicationControler::getInstance().sendSms("31654650997", "Hallo world!");

		//LocationTimeController::getInstance().addLocationListener(*this);
		MotionControler::getInstance().addMotionListener(*this);
	}

	void onMessageReceived(long long messageId, int responseStatus, const String &response) override{
		PRINTLN(String("id: ") + (int)messageId + " status: " + responseStatus + " message:\n" + response);
	}

	void onLocationFound(float latitude, float longitude) override{
		PRINTLN(String("Location: ") + String(latitude,6) + ", " + longitude);
		LocationTimeControler &lc = LocationTimeControler::getInstance();

		PRINTLN(String()+ lc.getHours() + ":" + lc.getMinutes() + ":" + lc.getSeconds());
	}

	void onMotion(const Motion &newMotion) override{
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
		Serial1.begin(9600);
		pinMode(2, OUTPUT);
		digitalWrite(2, HIGH);

		pinMode(23, OUTPUT);
		digitalWrite(23, LOW);
	}

	void run() override{
		while(true){
			HttpClient http(AtClient::getInstance());
			String rsp = http.post("http://www.sustain.net23.net/echo.php", "hallo world");
			PRINTLN(rsp);

			sleep(10000);
		}
	}
};

//Rules:
//Do not flush on setup.
//Tasks may not be allocated on stack.
void setup(){
	DEBUG_BEGIN(9600);

	new ComTest;
	//new TestTask;

	//WatchDog::getInstance().start(6000);

    TaskBase::startAllTasks();
}

void loop() {
	//not used
}
