#include <FreeRTOS_ARM.h>
#include <basic_io_arm.h>
#include "Aes.h"
#include "AtClient.h"
#include "HttpClient.h"
#include "DiagnosticsTool.h"
#include "Base64.h"
#include "TaskBase.h"
#include "TestTask.h"
#include "ArrayList.h"
#include "Debug.h"

void setup(){
	Serial.begin(9600);
	Serial1.begin(9600);
	Serial3.begin(9600);

	delay(100);
	Serial3.println("----------------------------");

 	AtClient at(Serial1);
 	HttpClient http(at);
 	Aes aes("this is a pasphrase yeee");

	// initialize semaphore
 	SemaphoreHandle_t sem = xSemaphoreCreateBinary();

 	TestTask *t1 = new TestTask(1);
 	//Serial3.println(String("ADDR T1 = ") + (int)t1);

 	TestTask *t2 = new TestTask(2);
 	//Serial3.println(String("ADDR T2 = ") + (int)t2);

 	TestTask *t3 = new TestTask(3);

 	TestTask *t4 = new TestTask(4);
	//Serial3.println(t2.getName());
 	//Serial3.flush();

    portBASE_TYPE s1, s2;

    //TODO: http://www.freertos.org/xTaskCreateRestricted.html

	// create task at priority two
	//s1 = xTaskCreate(runHelper, NULL, configMINIMAL_STACK_SIZE, t1, 1, NULL);

	// create task at priority one
	//s2 = xTaskCreate(runHelper, NULL, configMINIMAL_STACK_SIZE, t2, 2, NULL);

	//portBASE_TYPE s3 = xTaskCreate(foo, NULL, configMINIMAL_STACK_SIZE, NULL, 0, NULL);


	//Serial3.println(t1.getName() + " " + t1.getPiroirty());
	//Serial3.flush();

    //s1 = xTaskCreateRestricted( &xTaskDefinition, NULL );

	// check for creation errors
	//if (s1 != pdPASS || s2 != pdPASS ) {
	//	Serial3.println(F("Creation problem"));
	//	while(1);
	//}
	// start scheduler

    TaskBase::startAllTasks();


	while(1);

 	return;
 	String plainText = "Hallo world!";
 	while(plainText.length() % 16 != 0){
 		plainText += "x";
 	}
 	Serial3.println("Plain text: " + plainText);

 	unsigned char cipherText[plainText.length() + 1];
 	cipherText[plainText.length()] = '\0';
 	aes.encrypt((unsigned char*)plainText.c_str(), cipherText, plainText.length());

 	Serial3.println("Cipher text: " + String((char*)cipherText));

 	String base64 = base64_encode(cipherText, sizeof(cipherText));
 	Serial3.println("Base64 text: " + base64);

 	String notBase64 = base64_decode(base64);
 	Serial3.println("Cipher text: " + notBase64);

 	unsigned char decrptedText[notBase64.length() + 1];
 	decrptedText[notBase64.length()] = '\0';

 	aes.decrypt((unsigned char*)notBase64.c_str(), decrptedText, notBase64.length());


 	Serial3.println("Decripted text: " + String((char*)decrptedText));

 	return;

 	if(at.connect()){
 		String rdy;
 		Serial3.println(String(at.execute("AT+CPIN?", rdy)) + " | " + rdy);
 		Serial3.println(String(at.execute("AT+CSQ", rdy)) + " | " + rdy);
 		Serial3.println(String(at.execute("AT+CREG?", rdy)) + " | " + rdy);
 		Serial3.println(String(at.execute("AT+CGREG?", rdy)) + " | " + rdy);


 		for(int i=0; i<5 && !http.connect(); i++){
 			delay(1000);
 		}
 		Serial3.println(String(at.execute("AT+SAPBR=2,1", rdy)) + " | " + rdy);
 		Serial3.println("HTML:\r" + http.post("http://sustain.net23.net/echo.php?test=9", "test2=5", HttpClient::CONTENT_TYPE_POST));

 	}
 	else{
 		Serial3.println("AT failed");
 	}

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

	Serial3.println("Done");

	while(true){
		int read = Serial1.read();
		if(read != -1){
			Serial3.print((char)read);
		}
	}
}


void loop() {
  // Not used.
}
