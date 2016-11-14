
#include "AtClient.h"
#include "HttpClient.h"

#include "TestTask.h"
#include "Debug.h"

#include "CommunicationControler.h"
#include "LocationController.h"
#include "WatchDog.h"
#include "DateTimeControler.h"

#include "ArrayList.h"
#include "SustainWork.h"

class ComTest : public CommunicationListener, public LocationListener{
public:
	ComTest(){
		CommunicationControler::getInstance().sendPostRequest("http://google.nl", "hallo wrold", this);
		//CommunicationControler::getInstance().get("http://google.nl", this);
		CommunicationControler::getInstance().sendSms("31654650997", "Hallo world!");

		LocationController::getInstance().addLocationListener(*this);

	}

	void onMessageReceived(long long messageId, int responseStatus, const String &response) override{
		PRINTLN(String("id: ") + (int)messageId + " status: " + responseStatus + "message: " + response);
	}

	void onLocationFound(float latitude, float longitude) override{
		PRINTLN(String("Location: ") + latitude + ", " + longitude);
	}

};

//Rules:
//Do not flush on setup.
//Tasks may not be allocated on stack.
void setup(){
	DEBUG_BEGIN(9600);

	ComTest t;

	//DateTimeControler::getInstance();
	WatchDog::getInstance().start(6000);

	//WatchDog *watchdog = new WatchDog(6000); //Priority 4

	//CommunicationControler *comTask = new CommunicationControler(); //Priority 2
	//LocationController *locTask = new LocationController(); //Priority 3


 	//TestTask *t1 = new TestTask(1, *comTask);
 	//Serial3.println(String("ADDR T1 = ") + (int)t1);

 	TestTask *t2 = new TestTask(1);
 	//Serial3.println(String("ADDR T2 = ") + (int)t2);

 	TestTask *t3 = new TestTask(2);


    TaskBase::startAllTasks();

 	return;
	//while(1);
//
// 	return;
// 	String plainText = "Hallo world!";
// 	while(plainText.length() % 16 != 0){
// 		plainText += "x";
// 	}
// 	Serial3.println("Plain text: " + plainText);
//
// 	unsigned char cipherText[plainText.length() + 1];
// 	cipherText[plainText.length()] = '\0';
// 	aes.encrypt((unsigned char*)plainText.c_str(), cipherText, plainText.length());
//
// 	Serial3.println("Cipher text: " + String((char*)cipherText));
//
// 	String base64 = base64_encode(cipherText, sizeof(cipherText));
// 	Serial3.println("Base64 text: " + base64);
//
// 	String notBase64 = base64_decode(base64);
// 	Serial3.println("Cipher text: " + notBase64);
//
// 	unsigned char decrptedText[notBase64.length() + 1];
// 	decrptedText[notBase64.length()] = '\0';
//
// 	aes.decrypt((unsigned char*)notBase64.c_str(), decrptedText, notBase64.length());
//
//
// 	Serial3.println("Decripted text: " + String((char*)decrptedText));
//
// 	return;
//
// 	if(at.connect()){
// 		String rdy;
// 		Serial3.println(String(at.execute("AT+CPIN?", rdy)) + " | " + rdy);
// 		Serial3.println(String(at.execute("AT+CSQ", rdy)) + " | " + rdy);
// 		Serial3.println(String(at.execute("AT+CREG?", rdy)) + " | " + rdy);
// 		Serial3.println(String(at.execute("AT+CGREG?", rdy)) + " | " + rdy);
//
//
// 		for(int i=0; i<5 && !http.connect(); i++){
// 			delay(1000);
// 		}
// 		Serial3.println(String(at.execute("AT+SAPBR=2,1", rdy)) + " | " + rdy);
// 		Serial3.println("HTML:\r" + http.post("http://sustain.net23.net/echo.php?test=9", "test2=5", HttpClient::CONTENT_TYPE_POST));
//
// 	}
// 	else{
// 		Serial3.println("AT failed");
// 	}

	//Serial3.println(String(at.execute("AT")));
	//Serial3.println(String(at.execute("ATE1")));
	//Serial3.println(String(at.execute("AT")));


	//Serial3.println(String(at.execute("AT+CPIN")));

	//Serial3.println(at.e);

	//HttpClient http(at);
	//http.connect();
	//Serial3.println(http.getIp());

	//DiagnosticsTool diagnostic(at, http);
	//diagnostic.runDiagnostic(Serial3);


	//Serial3.println("Ip: " + http.getIp());
	//Serial3.println(http.get("http://google.nl"));

//	Serial3.println("Done");
//
//	while(true){
//		int read = Serial1.read();
//		if(read != -1){
//			Serial3.print((char)read);
//		}
//	}
}


void loop() {
	//not used
}
