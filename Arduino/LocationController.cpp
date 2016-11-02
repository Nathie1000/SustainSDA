/**
 * @file LocationController.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 1 nov. 2016
 */


#include "LocationController.h"
#include "PmtkClient.h"
#include "TaskBase.h"
#include "Debug.h"

LocationController::LocationController(int priority):
TaskBase(priority, "Location Task"),
pmtk(Serial3)
{

}

void LocationController::run(){
	PRINTLN("-----------------Location Task Start-----------");
	while(true){
		static bool once = true;
		if(once){
			once = false;
		}

		sleep(1000);
	}
}

