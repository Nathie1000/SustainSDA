/*
 * Composite.h
 *
 *  Created on: 14 okt. 2016
 *      Author: Nathan
 */

#ifndef COMPOSITEWAITABLE_H_
#define COMPOSITEWAITABLE_H_

#include "Waitable.h"
#include "ArrayList.h"

class CompositeWaitable{
private:
	ArrayList<const Waitable *> waitables;
public:
	const ArrayList<const Waitable *> getWaitables() const;
	void addWaitable(const Waitable &waitable);
	CompositeWaitable& operator|(const Waitable &other);

};



#endif /* COMPOSITEWAITABLE_H_ */
