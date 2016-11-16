/**
 * @file CompositeWaitable.h
 *
 * @author Nathan Schaaphuizen
 * @date 14 okt. 2016
 */

#ifndef COMPOSITEWAITABLE_H_
#define COMPOSITEWAITABLE_H_

#include "Waitable.h"
#include "ArrayList.h"

/**
 * @class CompositeWaitable
 * Helper class to composite multiple Waitable.
 * Every Waitable added to this class will be checked for a waiting state.
 */
class CompositeWaitable{
private:
	ArrayList<const Waitable *> waitables;
public:
	/**
	 * Get a list of Waitables.
	 * @return An ArrayList of Waitables.
	 */
	const ArrayList<const Waitable *> getWaitables() const;

	/**
	 * Add a waitable to internal list.
	 * @param waitable the Waitable to be added.
	 */
	void addWaitable(const Waitable &waitable);

	/**
	 * Operator to add a waitable to internal list.
	 * @param other the waitable to add.
	 * @return Reference to this object so the operator can be chained.
	 */
	CompositeWaitable& operator|(const Waitable &other);

};



#endif /* COMPOSITEWAITABLE_H_ */
