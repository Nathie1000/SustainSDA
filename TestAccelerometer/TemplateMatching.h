/**
* @file TemplateMatching.h
*
* @author Bianca Krieger
* @date 9 jan. 2017
*/

#ifndef _TEMPLATEMATCHING_
#define _TEMPLATEMATCHING_

#include "Algorithm.h"

/**
* @class TemplateMatching
* @brief This class is responsible for matching movements with templates. 
*		 Depending on the similarity of the movement and the template a percentage will be assigend.
*/

class TemplateMatching {
private:

	static const float templateWalking[];
	static const float templateFastWalking[];
	static const float templateCvaWalking[];

public:
	/**
	* @enum Movement
	*/
	enum class Movement{
		WALKING,			/*!< Template that consist a walking movement */
		FAST_WALKING,		/*!< Template that consist a fast phase walking movement */
		CVA_WAKLING			/*!< Template that consist a cva walking movement */
	};

	/**
	* @brief Scales the movement so that it can be matched against the template.
	* @param in The original size of the array of the movement.
	* @param out Scaled array of the movement.
	* @param newSize The new size that the in array must be scaled to.
	*/
	static void scale(const ArrayList<float> &in, ArrayList<float> &out, int newSize);

	/**
	* @brief Matches A movement with a template and assigns a percentage of similarity.
	* @param in An arraylist that containts the movement that will be matched with a template.
	* @param compareTemplate Is the template that will be machted with the movement.
	* @param templateSize The size of the template, needed to scale the movement.
	* @param maxDeviation The max deviation that can be assigned to a movement.
	* @return The percentage of classification.
	*/
	static float match(const ArrayList<float> &in, const float compareTemplate[], int templateSize, float maxDeviation);

	/**
	* @brief Function that matches the movement with all the templates.
	* @param vector The vector of the movement.
	* @param movement The movement of the template that the movement will me matched against.
	* @return The match function, and therefor the percentage of classification.
	*/
	static float matchTemplate(const ArrayList<float> vector, Movement movent);

};
#endif // _TEMPLATEMATCHING_


