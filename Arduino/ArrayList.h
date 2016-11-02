/**
 * @file AtClient.h
 *
 * @author Nathan Schaaphuizen
 * @date 11 okt. 2016
 */

#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_

#include "List.h"

/**
 * @class ArrayList
 * Utility class for dynamic array. This size of the array is dynamically grown and shrunk
 * as elements are added and removed. The elements are guaranteed to stay consecutive in memory.
 * Elements are stored by copy.
 * The max size of a list is theoretically the max amount of memory available. Use within reason!
 */
template <typename T> class ArrayList : public List<T>{
private:
	int index;
	int size;
	T *array;

public:

	/**
	 * Create new object.
	 * @param size the initial size of the list. Must be > 0.
	 */
	ArrayList(int size = 10) :
		index(0),
		size(size)
	{
		if (this->size < 1) this->size = 1;
		array = new T[this->size];
	}

	/**
	 * Create new object initialized by other list. Lists must be of same type.
	 * All fields are copied by value.
	 * @param other the other list to copy from.
	 */
	ArrayList(const ArrayList &other){
		index = other.index;
		size = other.size;
		array = new T[size];
		for (int i = 0; i < size; i++){
			array[i] = other[i];
		}
	}

	/**
	 * Destroy the object and free memory.
	 */
	~ArrayList(){
		delete[] array;
	}

	/**
	 * Get the number of elements in the list.
	 * @return the number of elements.
	 */
	int getSize() const override{
		return index;
	}

	/**
	 * Add element to the back of the list.
	 * @param obj element to add.
	 */
	void add(T obj) override{
		array[index] = obj;
		index++;
		if (index >= size){
			int newSize = size * 2;
			T *newArray = new T[newSize];
			for (int i = 0; i<index; i++){
				newArray[i] = array[i];
			}

			delete[] array;
			array = newArray;
			size = newSize;
		}
	}

	/**
	 * Add element to specific index. All elements on the right of the index are
	 * copied 1 position to the right.
	 * The index must already exits, else this function will do nothing.
	 * @param index the index to add the new element on.
	 * @param obj the element to be added.
	 */
	void addIndex(int index, T obj) override{
		if (index >= 0 && index < this->index){
			add(obj);
			for (int i = this->index - 1; i > index; i--){
				array[i] = array[i - 1];
			}
			array[index] = obj;
		}
	}

	/**
	 * Remove element from list.
	 * If element does not exist within list, this function does nothing.
	 * @param obj the element to be removed.
	 */
	void remove(T obj) override{
		for (int i = 0; i<index; i++){
			if (array[i] == obj){
				removeIndex(i);
				return;
			}
		}
	}

	/**
	 * Remove element from index.
	 * If index does not exits this, function does nothing.
	 * @param index the index of the element to be removed.
	 */
	void removeIndex(int index) override{
		if (index >= 0 && index < this->index){
			int newSize = size - 1;
			if (newSize > 0){
				for (int i = index; i<this->index; i++){
					if (i + 1 < size){
						array[i] = array[i + 1];
					}
				}
			}
			this->index--;
			if (size - this->index > this->index){
				int newSize = size / 2;
				if (newSize <= 0) newSize = 1;
				T *newArray = new T[newSize];
				for (int i = 0; i < newSize; i++){
					newArray[i] = array[i];
				}
				delete[] array;
				array = newArray;
				size = newSize;
			}
		}
	}

	/**
	 * Assign a new element to a index in the list.
	 * The index must exists else this function does nothing.
	 * @param index the index to be changed.
	 * @param obj the new element to be assigned to the index.
	 */
	void set(int index, T obj) const override{
		if (index >= 0 && index < this->index){
			array[index] = obj;
		}
	}

	/**
	 * Retrieve a element from the list.
	 * The index must exit else the behavior of this function is undefined.
	 * @param index the index to retrieve from.
	 * @return The element.
	 */
	T get(int index) const override{
		return array[index];
	}

	/**
	 * Convert the list to a C-type array. The array has the same length as the list.
	 * The array is a direct pointer to the inner array of the list and can be used for quick access.
	 * The list does not track any changes made to this array.
	 * USE AT YOUR OWN PERIL.
	 *
	 * @return Pointer to inner array of list.
	 */
	T* toArray() override{
		return array;
	}

	/**
	 * Convert the list to a constant C-type array. The array has the same length as the list.
	 * The array is a direct pointer to the inner array of the list and can be used for quick access.
	 * The list does not track any changes made to this array.
	 * USE AT YOUR OWN PERIL.
	 *
	 * @return Constant pointer to inner array of list.
	 */
	const T* toArray() const override{
		return array;
	}

	/**
	 * Left hand operator to access elements in list.
	 * @param index the index of element to retrieve.
	 * @return A reference to object in list.
	 */
	T& operator[](int index) override{
		return array[index];
	}

	/**
	 * Constant left hand operator to access elements in list.
	 * @param index the index of element to retrieve.
	 * @return A reference to object in list.
	 */
	const T& operator[](int index) const override{
		return array[index];
	}

	/**
	 * Member function to start the for each loop.
	 * Do not call.
	 * @return Pointer to first element in list.
	 */
	T* begin() override{
		return array;
	}

	/**
	 * Member function to start the constant for each loop.
	 * Do not call.
	 * @return Pointer to first element in list.
	 */
	const T* begin() const override{
		return array;
	}

	/**
	 * Member function to end the for each loop.
	 * Do not call.
	 * @return Pointer to an address one passed the last element in list.
	 */
	T* end() override{
		return &array[index];
	}

	/**
	 * Member function to end the constant for each loop.
	 * Do not call.
	 * @return Pointer to an address one passed the last element in list.
	 */
	const T* end() const override{
		return &array[index];
	}
};

#endif /* ARRAYLIST_H_ */
