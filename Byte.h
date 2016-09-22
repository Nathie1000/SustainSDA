/*
 * Byte.h
 *
 *  Created on: Sep 19, 2016
 *      Author: Bianca
 */

#ifndef BYTE_H_
#define BYTE_H_

#include <i2c_t3.h>

void readBytes(uint8_t address, uint8_t subAddress, uint8_t count,
		uint8_t * dest);
uint8_t readByte(uint8_t address, uint8_t subAddress);
void writeByte(uint8_t address, uint8_t subAddress, uint8_t data);
void M24512DFMreadBytes(uint8_t device_address, uint8_t data_address1,
		uint8_t data_address2, uint8_t count, uint8_t * dest);

#endif /* BYTE_H_ */
