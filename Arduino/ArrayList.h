/*
* ArrayList.h
*
*  Created on: 11 okt. 2016
*      Author: Nathan
*/

#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_

#include "List.h"

template <typename T> class ArrayList : public List<T>{
private:
	int index;
	int size;
	T *array;

public:

	ArrayList(int size = 10) :
		index(0),
		size(size)
	{
		if (this->size < 1) this->size = 1;
		array = new T[this->size];
	}

	ArrayList(const ArrayList &other){
		index = other.index;
		size = other.size;
		array = new T[size];
		for (int i = 0; i < size; i++){
			array[i] = other[i];
		}
	}

	~ArrayList(){
		delete[] array;
	}

	int getSize() const override{
		return index;
	}

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

	void addIndex(int index, T obj) override{
		if (index >= 0 && index < this->index){
			add(obj);
			for (int i = this->index - 1; i > index; i--){
				array[i] = array[i - 1];
			}
			array[index] = obj;
		}
	}

	void remove(T obj) override{
		for (int i = 0; i<index; i++){
			if (array[i] == obj){
				removeIndex(i);
				return;
			}
		}
	}

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

	void set(int index, T obj) const override{
		if (index >= 0 && index < this->index){
			array[index] = obj;
		}
	}

	T get(int index) const override{
		return array[index];
	}

	T* toArray() override{
		return array;
	}

	T& operator[](int index) override{
		return array[index];
	}

	const T& operator[](int index) const override{
		return array[index];
	}

	T* begin() override{
		return array;
	}

	const T* begin() const override{
		return array;
	}

	T* end() override{
		return &array[index];
	}

	const T* end() const override{
		return &array[index];
	}
};

#endif /* ARRAYLIST_H_ */
