/*
 * Sensor.h
 *
 *  Created on: 27 okt. 2016
 *      Author: Nathan
 */

#ifndef SENSOR_H_
#define SENSOR_H_

class Sensor{
public:
	virtual ~Sensor(){}
	virtual void update() = 0;
};


#endif /* SENSOR_H_ */
