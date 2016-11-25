/**
 * @file List.h
 *
 * @author Nathan Schaaphuizen
 * @date 11 okt. 2016
 */

#ifndef LIST_H_
#define LIST_H_

/**
 * @interface List
 * A interface that defines the minimum list functionality.
 */
template <typename T> class List{
public:
	/**
	 * Virtual deconstructor required by compiler, does nothing.
	 */
	virtual ~List(){};

	/**
	 * Prototype function to get the amount of items in the list.
	 * @return The amount of items.
	 */
	virtual int getSize() const = 0;

	/**
	 * Prototype function to add an item to the list.
	 * @param obj the item to be added.
	 */
	virtual void add(T obj) = 0;

	/**
	 * Prototype function to add a item to a specific index.
	 * @param index the index the item is added to.
	 * @param obj the object to be added
	 */
	virtual void addIndex(int index, T obj) = 0;

	/**
	 * Prototype function to remove a item from the list.
	 * @param obj the item to be removed.
	 */
	virtual void remove(T obj) = 0;

	/**
	 * Prototype function to remove a item from a specific index.
	 * @param index the index of the item to be removed.
	 */
	virtual void removeIndex(int index) = 0;

	/**
	 * Prototype function to set an index to a specific item.
	 * @param index the index to be set.
	 * @param obj the item to be added.
	 */
	virtual void set(int index, T obj) const = 0;

	/**
	 * Prototype function to get the item from a specific index.
	 * @param index the index to retrieve from.
	 * @return the item retrieved.
	 */
	virtual T get(int index) const = 0;

	/**
	 * Prototype function to remove all items in list.
	 */
	virtual void clear() = 0;

	/**
	 * Prototype function to convert the list to a C-type array.
	 * @return a C-type array made from the list.
	 */
	virtual T* toArray() = 0;

	/**
	 * Prototype function to convert the list to a constant C-type array.
	 * @return a constant C-type array made from the list.
	 */
	virtual const T* toArray() const = 0;

	/**
	 * Prototype operator to get an item from index.
	 * @param index the index to retrieve from.
	 * @return the item retrieved.
	 */
	virtual T& operator[](int index) = 0;

	/**
	 * Prototype operator to get an constant item from index.
	 * @param index the index to retrieve from.
	 * @return the item retrieved. This item may not be altered.
	 */
	virtual const T& operator[](int index) const = 0;

	/**
	 * Prototype function to start the for each loop.
	 * @return Pointer to first element in list.
	 */
	virtual T* begin() = 0;

	/**
	 * Prototype function to start the constant for each loop.
	 * @return Pointer to first element in list.
	 */
	virtual const T* begin() const = 0;

	/**
	 * Prototype function to end the for each loop.
	 * @return Pointer to an address one passed the last element in list.
	 */
	virtual T* end() = 0;

	/**
	 * Prototype function to end the constant for each loop.
	 * @return Pointer to an address one passed the last element in list.
	 */
	virtual const T* end() const = 0;
};

#endif // LIST_H
