/**
 * @file Queue.h
 *
 * @author Nathan Schaaphuizen
 * @date 21 okt. 2016
*/

#ifndef QUEUE_H_
#define QUEUE_H_

#include "Waitable.h"
#include <FreeRTOS_ARM.h>
#include "Debug.h"

/**
 * @class Queue
 * RTOS synchronization element.
 * An Queue can be used to pass data between task.
 * Multiple tasks can add to the Queue. Only on task can read the Queue.
 */
template<typename T> class Queue : public Waitable{
private:
	QueueHandle_t  queue;

public:

	/**
	 * Create a new object.
	 * Also registers the Queue within RTOS.
	 * @param length the amount of items that the Queue can hold.
	 */
	explicit Queue(int length):
		queue(NULL)
	{
		queue = xQueueCreate(length, sizeof(T));
		if(queue == NULL){
			PRINTLN("Error creating Queue.")
		}
	}

	/**
	 * Destroy the object.
	 * Also deregister the Queue within RTOS.
	 */
	~Queue(){
		if(queue != NULL){
			vQueueDelete(queue);
		}
	}

	/**
	 * Add a item the the back of the Queue.
	 * This function will block if the Queue is full.
	 * Items will be added by copy.
	 * @param obj the item to be added.
	 */
	void push(T obj){
		if(xQueueSend(queue, &obj, portMAX_DELAY) != pdPASS){
			PRINTLN("Error writing Queue.")
		}
	}

	/**
	 * Get and remove the first item in the Queue.
	 * This function will block if the Queue is empty.
	 * @return The first time in the Queue.
	 */
	T pop(){
		//T* recv = (T*) new char[sizeof(T)];
		char buffer[sizeof(T)];
		if(xQueueReceive(queue, buffer, portMAX_DELAY) != pdPASS){
			PRINTLN("Error reading Queue.");
		}
		return *reinterpret_cast<T*>((void*)buffer);
	}

	/**
	 * Get the amount of items in the Queue.
	 * @return the amount of items in the Queue.
	 */
	int peek() const{
		return uxQueueMessagesWaiting(queue);
	}

	/**
	 * Implementation of the Waitable interface.
	 * @return True if Queue is not empty, otherwise false.
	 */
	bool isWaiting() const override{
		return peek() == 0;
	}

};

#endif /* QUEUE_H_ */
