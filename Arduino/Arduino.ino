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

//#define portMPU_REGION_READ_WRITE				( 0x03UL << 24UL )

// The LED is attached to pin 13 on Arduino.
const uint8_t LED_PIN = 13;

static void foo(void *arg){
	while(true){
		Serial3.println("FOO");
		vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);
	}
}
// Declare a semaphore handle.
//SemaphoreHandle_t sem;
//------------------------------------------------------------------------------
/*
 * Thread 1, turn the LED off when signalled by thread 2.
 */
// Declare the thread function for thread 1.
static void Thread1(void* arg) {
  while (1) {
     //Serial3.println("T1");
    // Wait for signal from thread 2.
    //xSemaphoreTake(sem, portMAX_DELAY);
    vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);

    // Turn LED off.
    //digitalWrite(LED_PIN, LOW);
  }
}
//------------------------------------------------------------------------------
/*
 * Thread 2, turn the LED on and signal thread 1 to turn the LED off.
 */
// Declare the thread function for thread 2.
static void Thread2(void* arg) {

  pinMode(LED_PIN, OUTPUT);
  while (1) {
    Serial3.println("T2");
    int start = millis();
    vTaskDelay((2000L * configTICK_RATE_HZ) / 1000L);
    int end = millis();
    Serial3.println(String("Time") + (end - start));

    // Turn LED on.
    //digitalWrite(LED_PIN, HIGH);

    // Sleep for 200 milliseconds.
   // vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);

    // Signal thread 1 to turn LED off.
   // xSemaphoreGive(sem);

    // Sleep for 200 milliseconds.
    //vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);
  }
}






void setup(){
	Serial.begin(9600);
	Serial1.begin(9600);
	Serial3.begin(9600);

	//portNUM_CONFIGURABLE_REGIONS;
	//MemoryRegion_t

	delay(100);
	Serial3.println("----------------------------");
	PRINTLN("HAllo world from debug");
	PRINTLN(String("HAllo world from debug"));
	PRINTLN(999);

 	AtClient at(Serial1);
 	HttpClient http(at);
 	Aes aes("this is a pasphrase yeee");


	// initialize semaphore
	//sem = xSemaphoreCreateCounting(1, 0);

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

    //TaskBase::startAllTasks();


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

//------------------------------------------------------------------------------
// WARNING idle loop has a very small stack (configMINIMAL_STACK_SIZE)
// loop must never block
void loop() {
  // Not used.
}
