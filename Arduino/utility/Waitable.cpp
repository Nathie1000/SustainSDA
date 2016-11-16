/**
 * @file Waitable.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 14 okt. 2016
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
