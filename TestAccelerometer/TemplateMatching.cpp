#include "TemplateMatching.h"

const float TemplateMatching::templateWalking[] = {
	1179.83,
	1154.52,
	1159.6,
	1159.22,
	1087.03,
	958.9,
	968.44,
	944.55,
	925.55,
	926.71,
	940.25,
	945.14,
	945.56,
	968.68,
	990.43,
	1004.13
};

const float TemplateMatching::templateFastWalking[] = {
	1195.28,
	1385.31,
	1393.0,
	1314.38,
	1045.63,
	1001.68,
	825.03,
	753.48,
	799.73,
	968.35,
	1068.56
};

const float TemplateMatching::templateCvaWalking[] = {
		891.5,
		889.97,
		900.5,
		882.87,
		916.11,
		956.49,
		1083.62,
		1078.88,
		1128.02,
		1089.01,
		1070.17,
		969.55
};

void TemplateMatching::scale(const ArrayList<float> &in, ArrayList<float> &out, int newSize) {
	int oldSize = in.getSize() - 1;
	int originalSize = newSize - 1;

	//New size must be larger then 1.
	if (originalSize <= 0 || oldSize < 0) {
		return;
	}

	for (int i = 0; i < newSize; i++) {
		float scale = (i / (float)originalSize) * oldSize;
		float scale1 = 1 - (scale - floor(scale));
		float scale2 = scale - floor(scale);

		int oldI1 = (int)floor(scale);
		int oldI2 = oldI1 + 1;

		float newI = in[oldI1] * scale1 + in[oldI2] * scale2;
		out.add(newI);
	}
}

float TemplateMatching::match(const ArrayList<float> &in, const float compareTemplate[], int templateSize, float maxDeviation) {
	if (templateSize <= 0 || in.getSize() <= 0){
			return 0.0f;
	}

	ArrayList<float> scaledArray;
	scale(in, scaledArray, templateSize);
	float sum = 0;
	for (int i = 0; i < scaledArray.getSize(); i++){
		float div = abs(scaledArray[i] - compareTemplate[i]);

		float quartDeviation = 0.25f * maxDeviation;
		float halfDeviation = 0.5f * maxDeviation;
		float threeQuartDeviation = 0.75f * maxDeviation;

		if (div < quartDeviation){
			sum += 1;
		}
		else if (div < halfDeviation){
			sum += 0.75;
		}
		else if (div < threeQuartDeviation){
			sum += 0.5;
		}
		else if (div < maxDeviation){
			sum += 0.25;
		}
	}

	return sum / templateSize;
}

float TemplateMatching::matchTemplate(const ArrayList<float> vector, Movement movent){
	if(movent == Movement::WALKING){
		int size = sizeof(templateWalking) / sizeof(templateWalking[0]);
		return match(vector, templateWalking, size, 100);
	}
	else if(movent == Movement::FAST_WALKING){
		int size = sizeof(templateFastWalking) / sizeof(templateFastWalking[0]);
		return match(vector, templateFastWalking, size, 100);
	}
	else{
		int size = sizeof(templateCvaWalking) / sizeof(templateCvaWalking[0]);
		return match(vector, templateCvaWalking, size, 100);
	}
}
