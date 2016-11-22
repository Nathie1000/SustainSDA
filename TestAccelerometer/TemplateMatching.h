#ifndef _TEMPLATEMATCHING_
#define _TEMPLATEMATCHING_

#include "Algorithm.h"

class TemplateMatching {
	static const float templateAx[];
	static const float templateAy[];
	static const float templateAz[];
public:

	static float match(const ArrayList<float> &in, const float compareTemplate[], int templateSize);
	static void scale(const ArrayList<float> &in, ArrayList<float> &out, int newSize);
	static float matchTemplate(ArrayList<float> ax, ArrayList<float> ay, ArrayList<float> az);

};
#endif // _TEMPLATEMATCHING_


