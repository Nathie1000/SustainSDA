/**
 * @file Waitable.h
 *
 * @author Nathan Schaaphuizen
 * @date 14 okt. 2016
 */

#ifndef WAITABLE_H_
#define WAITABLE_H_

class CompositeWaitable;

/**
 * @interface Waitable
 * An abstract class to declare the Waitable functionality.
 * Any object that has a blocked state that requires waiting can implement this class
 * in order to be waited on in conjunction with other Waitables.
 */
class Waitable{
public:
	/**
	 * Virtual deconstructor required by compiler, does nothing.
	 */
	virtual ~Waitable();

	/**
	 * Function prototype to test if element is in a waiting state.
	 * This function does not block and returns immediately.
	 * @return True if waiting, false if not.
	 */
	virtual bool isWaiting() const = 0;

	/**
	 * Operator to easy create a CompositeWaitable.
	 * This and the other object will be added to the CompositeWaitable.
	 * @param other the other Waitable.
	 * @return A CompositeWaitable already containing this and the other object.
	 */
	CompositeWaitable operator|(const Waitable &other) const;

};



#endif /* WAITABLE_H_ */
