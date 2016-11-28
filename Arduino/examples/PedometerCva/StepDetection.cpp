#include "StepDetection.h"
#include "TemplateMatching.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <SustainWork.h>

StepDetection::StepDetection():
stepCount(0)
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
	StaticJsonBuffer<200> jsonBuffer;

	JsonObject& root = jsonBuffer.createObject();
	root["tel"] = "03461234567";
	//"YYYY-MM-DDThh:mm:ssTZD"
	root["time"] = "2016-07-21T05:23:00Z";
	root["steps"] = stepCount;
	String data;
	root.printTo(data);

	PRINTLN(data);
	CommunicationControler::getInstance().sendPostRequest("", data);
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


		if(isTimeToSend(60) || stepCount > 10){
			sendSteps();
			stepCount = 0;
		}

		mag.clear();
		ax.clear();
		ay.clear();
		az.clear();
	};
}
