/*
 * Waitable.cpp
 *
 *  Created on: 14 okt. 2016
 *      Author: Nathan
 */
#include "Waitable.h"
#include "CompositeWaitable.h"

Waitable::~Waitable(){

}

CompositeWaitable Waitable::operator|(const Waitable &other)const {
	CompositeWaitable compositeWaitable;
	compositeWaitable.addWaitable(*this);
	compositeWaitable.addWaitable(other);
	return compositeWaitable;
}
