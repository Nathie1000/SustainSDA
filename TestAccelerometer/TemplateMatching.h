#ifndef _TEMPLATEMATCHING_
#define _TEMPLATEMATCHING_

#include "Algorithm.h"

class TemplateMatching {
private:

	static const float templateWalking[];
	static const float templateFastWalking[];
	static const float templateCvaWalking[];

public:

	enum class Movement{
		WALKING,
		FAST_WALKING,
		CVA_WAKLING
	};


	static float match(const ArrayList<float> &in, const float compareTemplate[], int templateSize, float maxDeviation);
	static void scale(const ArrayList<float> &in, ArrayList<float> &out, int newSize);
	static float matchTemplate(const ArrayList<float> vector, Movement movent);

};
#endif // _TEMPLATEMATCHING_


