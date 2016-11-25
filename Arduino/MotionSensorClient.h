/**
 * @file MotionSensorClient.h
 *
 * @author Nathan Schaaphuizen
 * @date 14 nov. 2016
 */

#ifndef MOTIONSENSORCLIENT_H_
#define MOTIONSENSORCLIENT_H_

#include "Sensor.h"
#include "ArrayList.h"
#include "BMP280.h"
#include "EM7180.h"
#include "SENtral.h"
#include <i2c_t3.h>
#include "Eeprom.h"

/**
 * @interface MotionSensorListener
 * @brief Interface class to receive motion updates.
 */
class MotionSensorListener{
public:
	/**
	 * @struct Motion
	 * @brief a motion containing a single point for accelerometer, gyroscope and magnetometer.
	 */
	struct Motion{
		///The accelerometer x-axis.
		float ax;
		///The accelerometer y-axis.
		float ay;
		///The accelerometer z-axis.
		float az;

		///The gyroscope x-axis.
		float gx;
		///The gyroscope y-axis.
		float gy;
		///The gyroscope z-axis.
		float gz;

		///The magnetometer x-axis.
		float mx;
		///The magnetometer y-axis.
		float my;
		///The magnetometer z-axis.
		float mz;
	};

	/**
	 * Virtual deconstructor required by compiler, does nothing.
	 */
	virtual ~MotionSensorListener(){}

	/**
	 * Prototype function triggered when a new motion point is available.
	 * @param newMotion the new motion.
	 */
	virtual void onMotion(const Motion &newMotion) = 0;
};

/**
 * @class MotionSensorClient
 * @brief Class to communicate with the motion sensor.
 */
class MotionSensorClient : public Sensor{
private:
	ArrayList<MotionSensorListener*> motionListeners;
	BMP280 bmp;
	Eeprom eeprom;
	EM7180 em7180;
	SENtral sentral;
	int lastPollTime;
	bool isOpen;

public:
	/**
	 * Create a new object.
	 * @param groundPin pin set the low to serve as a ground.
	 * @param volt3V3Pin pin set to high to serve as supply.
	 * @param ic2Pins the pins used for I2C communication.
	 */
	MotionSensorClient(int groundPin, int volt3V3Pin, i2c_pins ic2Pins);

	/**
	 * Open device.
	 * @return True if device is successfully connected, false if not.
	 */
	bool openDevice();

	/**
	 * Test if device is still connected.
	 * @return True if device is still connected, false if it is not.
	 */
	bool isDeviceOpen();

	/**
	 * Implementation of the Sensor interface.
	 */
	void update() override;

	/**
	 * Add a MotionSensorListener.
	 * @param motionListener the MotionSensorListener to be added.
	 */
	void addMotionListener(MotionSensorListener &motionListener);
};

#endif /* MOTIONSENSORCLIENT_H_ */
