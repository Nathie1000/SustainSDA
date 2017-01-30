#include "Algorithm.h"

float Algorithm::magnitudeVector(float x, float y, float z) {
	return sqrt((x * x) + (y * y) + (z * z));
}

void Algorithm::movingAverage(const ArrayList<float> &inputArray,
		ArrayList<float> &outputArray, int16_t kernelSize) {
	float sum = 0;
	float inverseKernel = 1.0f / (float) kernelSize;
	for (int i = 0; i < inputArray.getSize() - kernelSize; i++) {
		for (int j = 0; j < kernelSize; j++) {
			sum += inputArray[j + i] * (inverseKernel);
		}
		outputArray.add(sum);
		sum = 0;
	}
}

float Algorithm::standardDeviation(const float data[], int sliceCount) {
	float sum = 0.0, mean, standardDeviation = 0.0;
	for (int16_t i = 0; i < sliceCount; ++i) {
		sum += data[i];
	}

	mean = sum / sliceCount;

	for (uint16_t i = 0; i < sliceCount; ++i) {
		standardDeviation += (data[i] - mean) * (data[i] - mean);
	}
	return sqrt(standardDeviation / sliceCount);
}

float Algorithm::standardDeviation(const ArrayList<float> data,
		int sliceCount) {
	return standardDeviation(data.toArray(), sliceCount);
}

float Algorithm::mean(const float data[], int sliceCount) {
	float sum = 0.0;
	for (int16_t i = 0; i < sliceCount; ++i)
		sum += data[i];

	return (sum / sliceCount);
}

float Algorithm::mean(const ArrayList<float> data, int sliceCount) {
	return mean(data.toArray(), sliceCount);
}

void Algorithm::peakDetection(const ArrayList<float> &data,
		ArrayList<Algorithm::Peak> &peaks, int peakThreshold,
		int lowThreshold) {
	for (int i = 1; i < data.getSize() - 1; i++) {
		//detect a peak above the given threshold
		if (data[i - 1] <= data[i] && data[i + 1] <= data[i]
				&& data[i] > peakThreshold) {
			Peak p;
			p.position = i;
			p.peak = 1;
			peaks.add(p);
		} else if (data[i - 1] >= data[i] && data[i + 1] >= data[i]
				&& data[i] < lowThreshold) {
			Peak p;
			p.position = i;
			p.peak = -1;
			peaks.add(p);
		} else {
			Peak p;
			p.position = i;
			p.peak = 0;
			peaks.add(p);
		}
	}
}

void Algorithm::isolateMovement(const ArrayList<float> &inputArrayMA, const ArrayList<Algorithm::Peak> &inputArray, int &firstPos, int &secondPos) {
	int firstPosition = -1;
	int secondPosition = -1;
	bool findPeakNext = false;

	//Find first peak or dal.
	for (int i = 0; i < inputArray.getSize(); i++) {
		if (inputArray[i].peak == 1) {
			findPeakNext = false;
			firstPosition = inputArray[i].position;
			break;
		} else if (inputArray[i].peak == -1) {
			findPeakNext = true;
			firstPosition = inputArray[i].position;
			break;
		}
	}

	//Nothing Found.
	if (firstPosition == -1) {
		return;
	}

	//Find second peak or dal.
	for (int i = firstPosition; i < inputArray.getSize(); i++) {
		if (findPeakNext) {
			if (inputArray[i].peak == -1) {
				secondPosition = inputArray[i].position;
				break;
			}
		} else if (inputArray[i].peak == 1) {
			secondPosition = inputArray[i].position;
			break;
		}
	}

	//Nothing Found.
	if (secondPosition == -1) {
		return;
	}

	//Find start of movment.
	bool foundFirst = false;
	for (int i = firstPosition; i >= 0; i--) {
		if (!findPeakNext && inputArrayMA[i] <= 1000) {
			firstPosition = i;
			foundFirst = true;
			break;
		} else if (findPeakNext && inputArrayMA[i] >= 1000) {
			firstPosition = i;
			foundFirst = true;
			break;
		}
	}

	//Found nothing.
	if (!foundFirst) {
		return;
	}

	//Find end of movment.
	bool foundLast = false;
	for (int i = firstPosition; i < inputArrayMA.getSize(); i++) {
		if (!findPeakNext && inputArrayMA[i] >= 1000) {
			secondPosition = i;
			foundFirst = true;
			break;
		} else if (findPeakNext && inputArrayMA[i] <= 1000) {
			secondPosition = i;
			foundFirst = true;
			break;
		}
	}

	if (!foundLast) {
		return;
	}
}
bool Algorithm::findMotion(float x, float y, float z, ArrayList<float> &ax, ArrayList<float> &ay, ArrayList<float> &az, ArrayList<float> &mag) {
	static bool foundStartMovement = false;
	static bool foundMiddle = false;
	static bool findLow = false;
	static ArrayList<float> inputArray;

	const float lowPass = 900.0f;
	const float highPass = 1100.0f;
	const float midPass = 1000.0f;

	float rawVector = magnitudeVector(x, y, z);
	inputArray.add(rawVector);
	if (inputArray.getSize() < 7) {
		return false;
	} else {
		inputArray.removeIndex(0);
	}

	ArrayList<float> outputArray;
	Algorithm::movingAverage(inputArray, outputArray, 5);
	float magVector = outputArray[0];
	//PRINTLN(magVector);

	if (ax.getSize() > 50) {
		foundStartMovement = false;
		foundMiddle = false;
		//PRINTLN("Time out!");
		return true;
	}

	if (foundStartMovement == false) {
		if (magVector <= lowPass) {
			foundStartMovement = true;
			ax.add(x);
			ay.add(y);
			az.add(z);
			mag.add(magVector);
			findLow = false;
			//PRINTLN(String("First ") + magVector + "lowpass");
		} else if (magVector >= highPass) {
			foundStartMovement = true;
			ax.add(x);
			ay.add(y);
			az.add(z);
			mag.add(magVector);
			findLow = true;
			//PRINTLN(String("First") + magVector + "highpass");
		}
		return false;
	} else if (foundMiddle == false) {
		ax.add(x);
		ay.add(y);
		az.add(z);
		mag.add(magVector);
		if (findLow && magVector <= midPass) {
			foundMiddle = true;
			//PRINTLN(String("Middle ") + magVector + " < 950");
		} else if (!findLow && magVector >= midPass) {
			foundMiddle = true;
			//PRINTLN(String("Middle ") + magVector + " > 1050");
		}
		return false;
	} else {
		ax.add(x);
		ay.add(y);
		az.add(z);
		mag.add(magVector);
		if (findLow && magVector >= midPass) {
			foundStartMovement = false;
			foundMiddle = false;
			//PRINTLN(String("Last ") + magVector);
			return true;
		} else if (!findLow && magVector <= midPass) {
			foundStartMovement = false;
			foundMiddle = false;
			//PRINTLN(String("Last ") + magVector);
			return true;
		}
	}
	return false;
}
