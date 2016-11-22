#include "StepDetection.h"
#include "TemplateMatching.h"

StepDetection::StepDetection() {
  MotionControler::getInstance().addMotionListener(*this);
}

ArrayList<float> mag;

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
		/*for (int i = 0; i < ax.getSize(); i++) {
			PRINT(mag[i]);
			PRINT("\t");
			PRINT(ax[i]);
			PRINT("\t");
			PRINT(ay[i]);
			PRINT("\t");
			PRINTLN(az[i]);
		}*/
		float stepPer = TemplateMatching::matchTemplate(ax, ay, ax);
		PRINTLN(String("Step: ") + stepPer);

		mag.clear();
		ax.clear();
		ay.clear();
		az.clear();
	};
	//TemplateMatching::match(ax,ay,az,0,0);

	if (inputArray.getSize() >= 50) {
		////Orgineel idee, hebben peak detection beetje aangepsast.
		//Algorithm::movingAverage(inputArray, outputArray, 5);
		//ArrayList<Algorithm::Peak> peaks;
		//Algorithm::peakDetection(outputArray, peaks, 1050, 1000);
		//Algorithm::stepDetection(peaks, count);

		//Serial.print("Steps: "); Serial.println(count);

		//inputArray.clear();
		//outputArray.clear();
	  }
}
