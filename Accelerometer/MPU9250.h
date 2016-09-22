/*
 * MPU9250.h
 *
 *  Created on: Sep 22, 2016
 *      Author: Bianca
 */

#ifndef MPU9250_H_
#define MPU9250_H_

#include "Byte.h"

#define SELF_TEST_X_GYRO 	0x00
#define SELF_TEST_Y_GYRO 	0x01
#define SELF_TEST_Z_GYRO 	0x02

#define SELF_TEST_X_ACCEL	0x0D
#define SELF_TEST_Y_ACCEL 	0x0E
#define SELF_TEST_Z_ACCEL	0x0F

#define SMPLRT_DIV       	0x19

#define CONFIG           	0x1A
#define GYRO_CONFIG      	0x1B
#define ACCEL_CONFIG     	0x1C
#define ACCEL_CONFIG2    	0x1D

#define ACCEL_XOUT_H     	0x3B
#define GYRO_XOUT_H      	0x43

#define MPU9250_ADDRESS          0x68   // Device address of MPU9250 when ADO = 0

uint8_t selfTest[6];
int16_t gAvg[3], aAvg[3], aSTAvg[3], gSTAvg[3];
float factoryTrim[6];
uint8_t FS;

void MPU9250SelfTest(float * destination);

#endif /* MPU9250_H_ */
