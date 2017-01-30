#include "StepDetection.h"
#include "TemplateMatching.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <SustainWork.h>
#include <TimeLib.h>

const String StepDetection::URL = "82.217.103.246/api/belt/sendPatientData";

StepDetection::StepDetection():
stepCount(0),
stepsToSend(0),
stepsToCount(0),
isSending(false)
{
	lastTime = millis();
	MotionControler::getInstance().addMotionListener(*this);
	CommunicationControler::getInstance();
}

bool StepDetection::isTimeToSend(int interval){
	int now = millis();
	if(now - lastTime >= interval){
		lastTime = now;
		return true;
	}
	return false;
}

void StepDetection::sendSteps(){
	isSending = true;
	stepsToCount += 10;

	StaticJsonBuffer<200> jsonBuffer;

	JsonObject& root = jsonBuffer.createObject();
	root["ccid"] = CommunicationControler::getInstance().getCcid();
	stepsToSend = stepCount;
	root["steps"] = stepsToSend;
	root["battery"] = 100;
	String data;
	root.printTo(data);
	CommunicationControler::getInstance().sendPostRequest(URL, data, this);
}

void StepDetection::onMessageReceived(long long messageId, int responseStatus, const String &response){
	if(responseStatus == 200){
		stepCount -= stepsToSend;
		stepsToCount = 10;
	}
	isSending = false;
}

void StepDetection::onMotion(const MotionSensorListener::Motion &newMotion) {
	static bool first = true;
	if (first) {
		first = false;
	}
	else if (Algorithm::findMotion(newMotion.ax, newMotion.ay, newMotion.az, ax, ay, az, mag)) {

		float stepWalk = TemplateMatching::matchTemplate(mag, TemplateMatching::Movement::WALKING);
		float stepFast = TemplateMatching::matchTemplate(mag, TemplateMatching::Movement::FAST_WALKING);
		float stepCva = TemplateMatching::matchTemplate(mag, TemplateMatching::Movement::CVA_WAKLING);

		if(stepWalk >= 0.60 || stepFast >= 0.60 || stepCva >= 0.60){
			stepCount += 1;
			PRINTLN(String("Steps: ") + stepCount);
		}

		if(((isTimeToSend(60000) && stepCount > 0)|| stepCount >= stepsToCount) && !isSending){
			sendSteps();
		}

		mag.clear();
		ax.clear();
		ay.clear();
		az.clear();
	};
}
