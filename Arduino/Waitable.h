/*
 * Waitable.h
 *
 *  Created on: 14 okt. 2016
 *      Author: Nathan
 */

#ifndef WAITABLE_H_
#define WAITABLE_H_

class CompositeWaitable;

class Waitable{
public:
	virtual ~Waitable();
	virtual bool isWaiting() const = 0;

	CompositeWaitable operator|(const Waitable &other) const;

};



#endif /* WAITABLE_H_ */
