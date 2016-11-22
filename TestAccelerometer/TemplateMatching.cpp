#include "TemplateMatching.h"

const float TemplateMatching::templateAx[] = {
	924.27,
	910.12,
	892.06,
	886.7,
	883.77,
	879.86,
	787.14,
	795.93,
	922.81,
	999.91,
	1042.37,
	1192.67,
	1347.86
};

const float TemplateMatching::templateAy[] = {
	367.95,
	361.12,
	246.93,
	296.7,
	284.5,
	239.12,
	218.14,
	155.18,
	194.71,
	260.59,
	419.19,
	203.5,
	-101.99

};

const float TemplateMatching::templateAz[] = {
	427,
	295.73,
	214.23,
	160.06,
	141.52,
	48.31,
	50.75,
	72.71,
	80.03,
	-26.35,
	-23.42,
	137.62,
	229.36
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

float TemplateMatching::match(const ArrayList<float> &in, const float compareTemplate[], int templateSize) {
	if (templateSize <= 0 || in.getSize() <= 0) {
		return 0.0f;
	}

	ArrayList<float> scaledArray;
	scale(in, scaledArray, templateSize);
	float sum = 0;
	for (int i = 0; i < scaledArray.getSize(); i++) {
		sum += fmin(scaledArray[i], compareTemplate[i]) / fmax(scaledArray[i], compareTemplate[i]);
	}

	return sum / templateSize;
}

float TemplateMatching::matchTemplate(ArrayList<float> ax, ArrayList<float> ay, ArrayList<float> az) {
	int templateSize = sizeof(templateAx) / sizeof(templateAx[0]);

	float sum = match(ax, templateAx, templateSize);
	sum += match(ay, templateAy, templateSize);
	sum += match(az, templateAz, templateSize);

	return sum / 3;
}