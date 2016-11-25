/**
 * @file Flag.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 14 okt. 2016
 */
#include "Flag.h"
#include <FreeRTOS_ARM.h>
#include <utility/event_groups.h>
#include "Debug.h"

Flag::Flag():
event(NULL)
{
	event = xEventGroupCreate();
	if(event == NULL){
		PRINTLN("Error creating Flag.");
	}
}

Flag::~Flag(){
	if(event != NULL){
		vEventGroupDelete(event);
	}
}

void Flag::set(){
	xEventGroupSetBits(event, 0x01);
}

void Flag::wait(){
	xEventGroupWaitBits(event, 0x01, pdTRUE, pdFALSE, portMAX_DELAY );
}

bool Flag::isWaiting() const{
	return !(xEventGroupGetBits(event) && 0x01);
}
