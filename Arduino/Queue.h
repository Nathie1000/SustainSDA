/*
 * Queue.h
 *
 *  Created on: 21 okt. 2016
 *      Author: Nathan
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include "Waitable.h"
#include <FreeRTOS_ARM.h>
#include "Debug.h"

template<typename T> class Queue : public Waitable{
private:
	QueueHandle_t  queue;

public:

	Queue(int length):
		queue(NULL)
	{
		queue = xQueueCreate(length, sizeof(T) );
		if(queue == NULL){
			PRINTLN("Error creating Queue.")
		}
	}

	~Queue(){
		if(queue != NULL){
			vQueueDelete(queue);
		}
	}

	void push(T obj){
		if(xQueueSend(queue, &obj, portMAX_DELAY) != pdPASS){
			PRINTLN("Error writing Queue.")
		}
	}

	T pop(){
		T* recv = (T*) new char[sizeof(T)];
		if(xQueueReceive(queue, recv, portMAX_DELAY) != pdPASS){
			PRINTLN("Error reading Queue.");
		}
		return *recv;
	}

	int peek() const{
		return uxQueueMessagesWaiting(queue);
	}

	bool isWaiting() const override{
		return peek() == 0;
	}

};




#endif /* QUEUE_H_ */
