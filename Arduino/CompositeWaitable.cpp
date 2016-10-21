/*
 * Composite.cpp
 *
 *  Created on: 14 okt. 2016
 *      Author: Nathan
 */
#include "CompositeWaitable.h"
#include "Waitable.h"
#include "ArrayList.h"

const ArrayList<const Waitable *> CompositeWaitable::getWaitables() const{
	return waitables;
}

void CompositeWaitable::addWaitable(const Waitable &waitable){
	waitables.add(&waitable);
}

CompositeWaitable& CompositeWaitable::operator|(const Waitable &other){
	addWaitable(other);
	return *this;
}
