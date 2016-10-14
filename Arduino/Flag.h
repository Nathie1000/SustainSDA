/*
 * Flag.h
 *
 *  Created on: 14 okt. 2016
 *      Author: Nathan
 */

#ifndef FLAG_H_
#define FLAG_H_

#include "Waitable.h"
#include <FreeRTOS_ARM.h>
#include <utility/event_groups.h>

class Flag : public Waitable{
private:
	EventGroupHandle_t event;

public:
	Flag();
	~Flag();

	void set();
	void wait();
	bool isWaiting() const override;
};

#endif /* FLAG_H_ */
