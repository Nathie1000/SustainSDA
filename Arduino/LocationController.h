/**
 * @file LocationController.h
 *
 * @author Nathan Schaaphuizen
 * @date 1 nov. 2016
 */

#ifndef LOCATIONCONTROLLER_H_
#define LOCATIONCONTROLLER_H_

#include "TaskBase.h"
#include "PmtkClient.h"

class LocationController : public TaskBase{
private:
	PmtkClient pmtk;

public:
	explicit LocationController(int priority);
	void run() override;

};



#endif /* LOCATIONCONTROLLER_H_ */
