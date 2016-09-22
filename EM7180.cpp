/*
 * EM7180.cpp
 *
 *  Created on: Sep 19, 2016
 *      Author: Bianca
 */

#include "EM7180.h"

void EM7180_set_gyro_FS(uint16_t gyro_fs) {
	uint8_t bytes[4], STAT;
	bytes[0] = gyro_fs & (0xFF);
	bytes[1] = (gyro_fs >> 8) & (0xFF);
	bytes[2] = 0x00;
	bytes[3] = 0x00;
	writeByte(EM7180_ADDRESS, EM7180_LoadParamByte0, bytes[0]); //Gyro LSB
	writeByte(EM7180_ADDRESS, EM7180_LoadParamByte1, bytes[1]); //Gyro MSB
	writeByte(EM7180_ADDRESS, EM7180_LoadParamByte2, bytes[2]); //Unused
	writeByte(EM7180_ADDRESS, EM7180_LoadParamByte3, bytes[3]); //Unused
	writeByte(EM7180_ADDRESS, EM7180_ParamRequest, 0xCB); //Parameter 75; 0xCB is 75 decimal with the MSB set high to indicate a paramter write processs
	writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x80); //Request parameter transfer procedure
	STAT = readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge); //Check the parameter acknowledge register and loop until the result matches parameter request byte
	while (!(STAT == 0xCB)) {
		STAT = readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge);
	}
	writeByte(EM7180_ADDRESS, EM7180_ParamRequest, 0x00); //Parameter request = 0 to end parameter transfer process
	writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x00); // Re-start algorithm
}

void EM7180_set_mag_acc_FS(uint16_t mag_fs, uint16_t acc_fs) {
	uint8_t bytes[4], STAT;
	bytes[0] = mag_fs & (0xFF);
	bytes[1] = (mag_fs >> 8) & (0xFF);
	bytes[2] = acc_fs & (0xFF);
	bytes[3] = (acc_fs >> 8) & (0xFF);
	writeByte(EM7180_ADDRESS, EM7180_LoadParamByte0, bytes[0]); //Mag LSB
	writeByte(EM7180_ADDRESS, EM7180_LoadParamByte1, bytes[1]); //Mag MSB
	writeByte(EM7180_ADDRESS, EM7180_LoadParamByte2, bytes[2]); //Acc LSB
	writeByte(EM7180_ADDRESS, EM7180_LoadParamByte3, bytes[3]); //Acc MSB
	writeByte(EM7180_ADDRESS, EM7180_ParamRequest, 0xCA); //Parameter 74; 0xCA is 74 decimal with the MSB set high to indicate a paramter write processs
	writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x80); //Request parameter transfer procedure
	STAT = readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge); //Check the parameter acknowledge register and loop until the result matches parameter request byte
	while (!(STAT == 0xCA)) {
		STAT = readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge);
	}
	writeByte(EM7180_ADDRESS, EM7180_ParamRequest, 0x00); //Parameter request = 0 to end parameter transfer process
	writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x00); // Re-start algorithm
}

void EM7180_set_integer_param(uint8_t param, uint32_t param_val) {
	uint8_t bytes[4], STAT;
	bytes[0] = param_val & (0xFF);
	bytes[1] = (param_val >> 8) & (0xFF);
	bytes[2] = (param_val >> 16) & (0xFF);
	bytes[3] = (param_val >> 24) & (0xFF);
	param = param | 0x80; //Parameter is the decimal value with the MSB set high to indicate a paramter write processs
	writeByte(EM7180_ADDRESS, EM7180_LoadParamByte0, bytes[0]); //Param LSB
	writeByte(EM7180_ADDRESS, EM7180_LoadParamByte1, bytes[1]);
	writeByte(EM7180_ADDRESS, EM7180_LoadParamByte2, bytes[2]);
	writeByte(EM7180_ADDRESS, EM7180_LoadParamByte3, bytes[3]); //Param MSB
	writeByte(EM7180_ADDRESS, EM7180_ParamRequest, param);
	writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x80); //Request parameter transfer procedure
	STAT = readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge); //Check the parameter acknowledge register and loop until the result matches parameter request byte
	while (!(STAT == param)) {
		STAT = readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge);
	}
	writeByte(EM7180_ADDRESS, EM7180_ParamRequest, 0x00); //Parameter request = 0 to end parameter transfer process
	writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x00); // Re-start algorithm
}



