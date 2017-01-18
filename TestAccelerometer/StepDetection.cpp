#include "StepDetection.h"
#include "TemplateMatching.h"

StepDetection::StepDetection() {
	MotionControler::getInstance().addMotionListener(*this);
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
	//PRINTLN(String("Gyro: ") + newMotion.gx + " "+ newMotion.gy + " " + newMotion.gz);
	//return;

	else if (Algorithm::findMotion(newMotion.ax, newMotion.ay, newMotion.az, ax, ay, az, mag)) {
		PRINTLN("----------------------");
		PRINTLN("ax \t ay \t az \t gx \t gy \t gz");
		for (int i = 0; i < ax.getSize(); i++) {
			PRINT(ax[i]);
			PRINT("\t");
			PRINT(ay[i]);
			PRINT("\t");
			PRINT(az[i]);
		}

//		float stepPer = TemplateMatching::matchTemplate(mag, TemplateMatching::Movement::WALKING);
//		PRINTLN(String("Step: ") + stepPer);
//
//		stepPer = TemplateMatching::matchTemplate(mag, TemplateMatching::Movement::FAST_WALKING);
//		PRINTLN(String("Fast Step: ") + stepPer);
//
//		stepPer = TemplateMatching::matchTemplate(mag, TemplateMatching::Movement::CVA_WAKLING);
//		PRINTLN(String("Cva Step: ") + stepPer);

		mag.clear();
		ax.clear();
		ay.clear();
		az.clear();
	};
}
