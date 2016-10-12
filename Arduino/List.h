#ifndef LIST_H_
#define LIST_H_

template <typename T> class List{
public:
	virtual ~List(){};
	virtual int getSize() const = 0;

	virtual void add(T obj) = 0;
	virtual void addIndex(int index, T obj) = 0;

	virtual void remove(T obj) = 0;
	virtual void removeIndex(int index) = 0;

	virtual void set(int index, T obj) const = 0;
	virtual T get(int index) const = 0;

	virtual T* toArray() = 0;

	virtual T& operator[](int index) = 0;
	virtual const T& operator[](int index) const = 0;

	virtual T* begin() = 0;
	virtual const T* begin() const = 0;

	virtual T* end() = 0;
	virtual const T* end() const = 0;
};


#endif // LIST_H
