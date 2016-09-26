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

#define XG_OFFSET_H      	0x13  // User-defined trim values for gyroscope
#define XG_OFFSET_L      	0x14
#define YG_OFFSET_H      	0x15
#define YG_OFFSET_L      	0x16
#define ZG_OFFSET_H      	0x17
#define ZG_OFFSET_L      	0x18
#define SMPLRT_DIV       	0x19

#define CONFIG           	0x1A
#define GYRO_CONFIG      	0x1B
#define ACCEL_CONFIG     	0x1C
#define ACCEL_CONFIG2    	0x1D

#define FIFO_EN          	0x23
#define I2C_MST_CTRL     	0x24
#define INT_PIN_CFG      	0x37
#define INT_ENABLE       	0x38

#define ACCEL_XOUT_H     	0x3B
#define GYRO_XOUT_H      	0x43

#define USER_CTRL        	0x6A  // Bit 7 enable DMP, bit 3 reset DMP
#define PWR_MGMT_1      	0x6B // Device defaults to the SLEEP mode
#define PWR_MGMT_2       	0x6C

#define MPU9250_ADDRESS     0x68   // Device address of MPU9250 when ADO = 0

#define FIFO_COUNTH      	0x72
#define FIFO_R_W         	0x74

#define XA_OFFSET_H      	0x77
#define YA_OFFSET_H      	0x7A
#define ZA_OFFSET_H      	0x7D

//uint8_t rawData[6];
static uint8_t selfTest[6];
static int16_t gAvg[3], aAvg[3], aSTAvg[3], gSTAvg[3];
static float factoryTrim[6];
static uint8_t FS;

static uint8_t Gscale;
static uint8_t Ascale;
static float aRes, gRes;

void getGres();
void getAres();

void MPU9250SelfTest(float * destination);
void accelgyrocalMPU9250(float * dest1, float * dest2);
void initMPU9250();

#endif /* MPU9250_H_ */
