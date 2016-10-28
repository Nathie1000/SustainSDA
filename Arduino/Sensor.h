/**
 * @file Sensor.h
 *
 * @author Nathan Schaaphuizen
 * @date 27 okt. 2016
 */

#ifndef SENSOR_H_
#define SENSOR_H_

/**
 * @interface Sensor
 * Interface to declare the Sensor functionality.
 */
class Sensor{
public:
	/**
	 * Virtual deconstructor required by compiler, does nothing.
	 */
	virtual ~Sensor(){}

	/**
	 * Prototype function periodically called to update the Sensor.
	 */
	virtual void update() = 0;
};


#endif /* SENSOR_H_ */
