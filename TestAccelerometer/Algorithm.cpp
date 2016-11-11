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

//void Algorithm::peakDetection(const ArrayList<float> y, int lag, float thresholds, float influence){
//	int8_t * signals;
//	float * filteredY;
//	signals = new int8_t[y.getSize()]{ };
//	filteredY = new float[y.getSize()]{ };
//
//	for (int i = 0; i < lag; i++)
//	{
//		filteredY[i] = y[i];
//	}
//
//	// Initialise filters
//
//	float *avgFilter, *stdFilter;
//	avgFilter = new float[y.getSize()];
//	stdFilter = new float[y.getSize()];
//
//	// Initialize first values
//	avgFilter[lag] = mean(y, lag);
//	stdFilter[lag] = standardDeviation(y, lag);
//
//	for (int i = lag + 1; i < y.getSize(); i++){
//		if (abs(y[i] - avgFilter[i - 1]) > thresholds*stdFilter[i - 1]){
//			if (y[i] > avgFilter[i - 1]) {
//				signals[i] = 1;
//			}else {
//				signals[i] = -1;
//			}
//
//			//Adjust the filters
//			filteredY[i] = influence * y[i] + (1 - influence)*filteredY[i - 1];
//			avgFilter[i] = mean((filteredY + i - lag), lag);
//			stdFilter[i] = standardDeviation((filteredY + i - lag), lag);
//		}
//		else {
//			signals[i] = 0; // no signal
//							  //adjust filters
//			filteredY[i] = y[i];
//			avgFilter[i] = mean(filteredY + i - lag, lag);
//			stdFilter[i] = standardDeviation(filteredY + i - lag, lag);
//		}
//	}
//
//	for (int i = 0; i < y.getSize(); i++)
//	{
//		Serial.print(String("") + y[i] + "\t");
//		Serial.print(avgFilter[i]); Serial.print("\t");
//		Serial.print(stdFilter[i]); Serial.print("\t"); 
//		Serial.println(signals[i]);
//		
//
//	}
//
//	Serial.println("einde data!!!");
//}

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

	//for(int i = 0; i < peaks.getSize(); i++) {
	//	Serial.println(peaks[i].peak);
	//}
	////Fuse nearby peaks together.
	//for(int i = 0; i < peaks.getSize(); i++) {
	//	if(peaks[i].peak == 1){
	//		int highestPeak = data[peaks[i].position];
	//		float higestPeakPosition = peaks[i].position;
	//		//Look 5 peaks into the future.
	//		for(int j = 0; j <5; j++){
	//			if(peaks[i+j].peak == 1){
	//				peaks[i+j].peak = 0;
	//				if(data[peaks[i+j].position] > highestPeak){
	//					highestPeak = data[peaks[i+j].position];
	//					higestPeakPosition = i + j;
	//				}
	//			}
	//		}
	//		//Amount of steps in the future - 1, cuz the loop does +1 on its own.
	//		i += 4;
	//		peaks[higestPeakPosition].peak = 1;
	//	}
	//}
	//Serial.println("-------------------------------------------");
	//for(int i = 0; i < peaks.getSize(); i++) {
	//		Serial.println(peaks[i].peak);
	//	}
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