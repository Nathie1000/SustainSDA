#include "StepDetection.h"
#include "TemplateMatching.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <SustainWork.h>
#include <TimeLib.h>

StepDetection::StepDetection():
stepCount(0),
stepsToSend(0),
isSending(false)
{
	lastTime = second();
	MotionControler::getInstance().addMotionListener(*this);
}

bool StepDetection::isTimeToSend(int interval){
	int now = second();
	if(now - lastTime >= interval){
		lastTime = now;
		return true;
	}
	return false;
}

void StepDetection::sendSteps(){
	isSending = true;
	StaticJsonBuffer<200> jsonBuffer;

	JsonObject& root = jsonBuffer.createObject();
	root["ccid"] = CommunicationControler::getInstance().getCcid();
	stepsToSend = stepCount;
	root["steps"] = stepsToSend;
	String data;
	root.printTo(data);
	PRINTLN(data);
	//TODO: add url here!
	CommunicationControler::getInstance().sendPostRequest("", data, this);
}

void StepDetection::onMessageReceived(long long messageId, int responseStatus, const String &response){
	if(responseStatus == 200){
		stepCount -= stepsToSend;
	}
	isSending = false;
}

void StepDetection::onMotion(const MotionSensorListener::Motion &newMotion) {
	static bool first = true;
	if (first) {
		first = false;
	}
	//PRINTLN(Algorithm::magnitudeVector(newMotion.ax, newMotion.ay, newMotion.az));
	//return;
	//ax.add(newMotion.ax);
	//ay.add(newMotion.ay);
	//az.add(newMotion.az);
	//inputArray.add(Algorithm::magnitudeVector(newMotion.ax, newMotion.ay, newMotion.az));
	else if (Algorithm::findMotion(newMotion.ax, newMotion.ay, newMotion.az, ax, ay, az, mag)) {
		PRINTLN("----------------------");
//		for (int i = 0; i < ax.getSize(); i++) {
//			PRINT(mag[i]);
//			PRINT("\t");
//			PRINT(ax[i]);
//			PRINT("\t");
//			PRINT(ay[i]);
//			PRINT("\t");
//			PRINTLN(az[i]);
//		}

		float stepPer = TemplateMatching::matchTemplate(mag, TemplateMatching::Movement::WALKING);
		PRINTLN(String("Step: ") + stepPer);

		stepPer = TemplateMatching::matchTemplate(mag, TemplateMatching::Movement::FAST_WALKING);
		PRINTLN(String("Fast Step: ") + stepPer);

		stepPer = TemplateMatching::matchTemplate(mag, TemplateMatching::Movement::CVA_WAKLING);
		PRINTLN(String("Cva Step: ") + stepPer);


		if((isTimeToSend(60) || stepCount > 10) && !isSending){
			sendSteps();
		}

		mag.clear();
		ax.clear();
		ay.clear();
		az.clear();
	};
}
