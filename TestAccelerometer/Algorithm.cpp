#include "Algorithm.h"


float Algorithm::magnitudeVector(float x, float y, float z) {
	return sqrt((x*x) + (y*y) + (z*z));
}

void Algorithm::movingAverage(const ArrayList<float> &inputArray, ArrayList<float> &outputArray, int16_t kernelSize) {
	float sum = 0;
	float inverseKernel = 1.0f / (float)kernelSize;
	for (int i = 0; i < inputArray.getSize()- kernelSize; i++){
		for (int j = 0; j < kernelSize; j++){
			sum += inputArray[j+i] * (inverseKernel);	
		}
		outputArray.add(sum);
		sum = 0;
	}
}

float Algorithm::standardDeviation(const float data[], int sliceCount){
	float sum = 0.0, mean, standardDeviation = 0.0;
	for (int16_t i = 0; i < sliceCount; ++i){
		sum += data[i];
	}

	mean = sum / sliceCount;

	for (uint16_t i = 0; i < sliceCount; ++i) {
		standardDeviation += (data[i] - mean) * (data[i] - mean);
	}
	return sqrt(standardDeviation / sliceCount);
}

float Algorithm::standardDeviation(const ArrayList<float> data, int sliceCount) {
	return standardDeviation(data.toArray(), sliceCount);
}

float Algorithm::mean(const float data[], int sliceCount)
{
	float sum = 0.0;
	for (int16_t i = 0; i < sliceCount; ++i)
		sum += data[i];

	return (sum / sliceCount);
}

float Algorithm::mean(const ArrayList<float> data, int sliceCount) {
	return mean(data.toArray(), sliceCount);
}

void Algorithm::peakDetection(const ArrayList<float> &data, ArrayList<Algorithm::Peak> &peaks, int peakThreshold, int lowThreshold) {
	for (int i = 1; i < data.getSize() - 1; i++) {
		//detect a peak above the given threshold
		if (data[i - 1] <= data[i] && data[i + 1] <= data[i] && data[i] > peakThreshold) {
			Peak p;
			p.position = i;
			p.peak = 1;
			peaks.add(p);
		}
		else if (data[i - 1] >= data[i] && data[i + 1] >= data[i] && data[i] < lowThreshold) {
			Peak p;
			p.position = i;
			p.peak = -1;
			peaks.add(p);
		}
		else {
			Peak p;
			p.position = i;
			p.peak = 0;
			peaks.add(p);
		}
	}
}

//When a high peak is detected, look voor a low peak. When this occured a step is been set. 
//continu to look voor a high peak again.
void Algorithm::stepDetection(ArrayList<Algorithm::Peak>& peaks, uint16_t & stepCount){
	for (int i = 0; i < peaks.getSize(); i++) {
		if (peaks[i].peak == 1) {
			for (i; i < peaks.getSize(); i++) {
				if (peaks[i].peak == -1) {
					stepCount++;
					break;
				}
			}
		}
	}
}