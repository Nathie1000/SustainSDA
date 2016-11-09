#ifndef _ALGORITHM_H
#define _ALGORITHM_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "ArrayList.h"
#include <math.h>
class Algorithm {
public:
	struct Peak {
		uint16_t position;
		int16_t peak;

		bool operator==(const Peak& other) { return position == other.position && peak == other.peak; };
	};

	static float magnitudeVector(float x, float y, float z);
	static void movingAverage(const ArrayList<float> &inputArray, ArrayList<float> &outputArray, int16_t kernelSize);
	
	static float standardDeviation(const float data[], int sliceCount);
	static float standardDeviation(const ArrayList<float> data, int sliceCount);
	
	static float mean(const float data[], int sliceCount);
	static float mean(const ArrayList<float> data, int sliceCount);

	static void peakDetection(const ArrayList<float> &data, ArrayList<Algorithm::Peak> &peaks, int peakThreshold, int lowThreshold);
};

#endif
