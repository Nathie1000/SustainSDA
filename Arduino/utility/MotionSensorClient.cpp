/**
 * @file MotionSensorClient.cpp
 *
 * @author Nathan Schaaphuizen
 * @date 14 nov. 2016
 */

#include "MotionSensorClient.h"
#include "Sensor.h"
#include "ArrayList.h"
#include <Arduino.h>
#include "PollControler.h"
#include "BMP280.h"
#include "DefineAddress.h"
#include "EM7180.h"
#include "SENtral.h"
#include <i2c_t3.h>

#include "Eeprom.h"
#include "ReadWriteByte.h"

uint16_t EM7180_mag_fs, EM7180_acc_fs, EM7180_gyro_fs; // EM7180 sensor full scale ranges
float gyroBias[3] = { 0, 0, 0 }, accelBias[3] = { 0, 0, 0 }, magBias[3] = { 0, 0, 0 }, magScale[3] = { 0, 0, 0 };  // Bias corrections for gyro, accelerometer, mag
float accelX, accelY, accelZ, gyroX, gyroY, gyroZ, magX, magY, magZ; // variables to hold latest sensor data values
uint32_t lastUpdate = 0;
float sum = 0;

// MPU9250 variables
int16_t accelCount[3] = { 0, 0, 0 };  // Stores the 16-bit signed accelerometer sensor output
int16_t gyroCount[3] = { 0, 0, 0 };   // Stores the 16-bit signed gyro sensor output
int16_t magCount[3] = { 0, 0, 0 };    // Stores the 16-bit signed magnetometer sensor output
float Quat[4] = { 0, 0, 0, 0 }; // quaternion data register


MotionSensorClient::MotionSensorClient(int groundPin, int volt3V3Pin, i2c_pins ic2Pins):
lastPollTime(0),
isOpen(false)
{
	PollControler::getInstance().addSensor(*this);
	// setup for Master mode, pins 18/19, external pullups, 400kHz for Teensy 3.1
	Wire.begin(I2C_MASTER, 0x00, ic2Pins, I2C_PULLUP_EXT, I2C_RATE_400);

	// Set up the interrupt pin, its set as active high, push-pull
	//Ground
	pinMode(groundPin, OUTPUT);
	digitalWrite(groundPin, LOW);

	//3V3
	pinMode(volt3V3Pin, OUTPUT);
	digitalWrite(volt3V3Pin, HIGH);

}

bool MotionSensorClient::openDevice(){
	//Should detect the SENtral at 0x28
	//Scan must be perfomed for some reason.

	if(ReadWriteByte::I2Cscan()){
		//sentral.readSENtralDevices();
		//em7180.SensorsDetected();

		isOpen = eeprom.initEEPROM();
		em7180.EM7180InitState();

		em7180.readSensorFS(EM7180_mag_fs, 0x4A, 1);
		em7180.readSensorFS(EM7180_acc_fs, 0x4A, 3);
		em7180.readSensorFS(EM7180_gyro_fs, 0x4B, 1);

		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_ParamRequest, 0x00); //End parameter transfer
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x00); // re-enable algorithm

		// If enabled the device will reduce it power when the gyro and magnetometer are "still" for an x amount of time.
		em7180.EM7180SetIntInParam(0x49, 0x00); //Disable stillness mode
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		//Write desired sensor full scale ranges to the EM7180
		em7180.EM7180SetModuleAccFS(0x3E8, 0x08, 0xCA);
		em7180.EM7180SetModuleAccFS(0x7D0, 0x00, 0xCB);

		//Read the new FS values
		em7180.readSensorFS(EM7180_mag_fs, 0x4A, 1);
		em7180.readSensorFS(EM7180_acc_fs, 0x4A, 3);
		em7180.readSensorFS(EM7180_gyro_fs, 0x4B, 1);

		em7180.EM7180Status();
		em7180.sensorState();
	}

	return isOpen;
}

void MotionSensorClient::update(){
	if(!isOpen) return;
	int nowTime = millis();
	if(nowTime - lastPollTime >= 50){
		lastPollTime = nowTime;
		int16_t rawPressure = 0, rawTemperature = 0;   // pressure, temperature raw count output
		float pressure = 0.0f;
		float temperature = 0.0f;

		//Check event status register, way to check data ready by polling rather than interrupt
		uint8_t eventStatus = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_EventStatus); // reading clears the register

		 //Check for errors
		if (eventStatus & 0x02) { // error detected, what is it?
			sentral.error();
		}
		//if no errors, see if new data is ready
		if (eventStatus & 0x10) { // new acceleration data available
			sentral.newData(accelCount, EM7180_AX, accelX, accelY, accelZ, 0.000488f);
			/*if (accelX != oldX || accelY != oldY || accelZ != oldZ) {
				oldX = accelX;
				oldY = accelY;
				oldZ = accelZ;
			}*/
		}
		if (eventStatus & 0x20) { // new gyro data available
			sentral.newData(gyroCount, EM7180_GX, gyroX, gyroY, gyroZ, 0.153f);
		}
		if (eventStatus & 0x08) { // new mag data available
			sentral.newData(magCount, EM7180_MX, magX, magY, magZ, 0.305176f);
		}
		if (eventStatus & 0x04) { // new quaternion data available
			sentral.readSENtralQuatData(Quat);
		}
		//get BMP280 pressure
		if (eventStatus & 0x40) { // new baro data available
			bmp.newData(rawPressure, pressure, rawTemperature, temperature, sentral);
		}

		/*keep track of rates*/
		uint32_t Now = micros();
		float deltat = ((Now - lastUpdate) / 1000000.0f); // set integration time by time elapsed since last filter update
		lastUpdate = Now;

		sum += deltat; // sum for averaging filter update rate

		//Sensors x (y)-axis of the accelerometer/gyro is aligned with the y (x)-axis of the magnetometer;
		//the magnetometer z-axis (+ down) is misaligned with z-axis (+ up) of accelerometer and gyro!
		//We have to make some allowance for this orientation mismatch in feeding the output to the quaternion filter.
		//We will assume that +y accel/gyro is North, then x accel/gyro is East. So if we want te quaternions properly aligned
		//we need to feed into the madgwick function Ay, Ax, -Az, Gy, Gx, -Gz, Mx, My, and Mz. But because gravity is by convention
		//positive down, we need to invert the accel data, so we pass -Ay, -Ax, Az, Gy, Gx, -Gz, Mx, My, and Mz into the Madgwick
		//function to get North along the accel +y-axis, East along the accel +x-axis, and Down along the accel -z-axis.
		//This orientation choice can be modified to allow any convenient (non-NED) orientation convention.
		//This is ok by aircraft orientation standards!
		//Pass gyro rate as rad/s
		eeprom.MadgwickQuaternionUpdate(-accelY, -accelX, accelZ, gyroY * PI / 180.0f, gyroX * PI / 180.0f, -gyroZ * PI / 180.0f, magX, magY, magZ, deltat);

		MotionSensorListener::Motion motion;
		motion.ax = 1000 * accelX;
		motion.ay = 1000 * accelY;
		motion.az = 1000 * accelZ;

		motion.gx = gyroX;
		motion.gy = gyroY;
		motion.gz = gyroZ;

		motion.mx = magX;
		motion.my = magY;
		motion.mz = magZ;

		for(MotionSensorListener *motionListener : motionListeners){
			motionListener->onMotion(motion);
		}
	}
}

bool MotionSensorClient::isDeviceOpen(){
	return isOpen;
}

void MotionSensorClient::addMotionListener(MotionSensorListener &motionListener){
	motionListeners.add(&motionListener);
}
