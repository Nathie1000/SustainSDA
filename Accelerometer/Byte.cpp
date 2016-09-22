/*
 * Byte.cpp
 *
 *  Created on: Sep 19, 2016
 *      Author: Bianca
 */

#include "Byte.h"

void readBytes(uint8_t address, uint8_t subAddress, uint8_t count,
		uint8_t * dest) {
	Wire.beginTransmission(address);   // Initialize the Tx buffer
	Wire.write(subAddress);           // Put slave register address in Tx buffer
	Wire.endTransmission(I2C_NOSTOP); // Send the Tx buffer, but send a restart to keep connection alive
//	Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
	uint8_t i = 0;
//        Wire.requestFrom(address, count);  // Read bytes from slave register address
	Wire.requestFrom(address, (size_t) count); // Read bytes from slave register address
	while (Wire.available()) {
		dest[i++] = Wire.read();
	}         // Put read results in the Rx buffer
}

uint8_t readByte(uint8_t address, uint8_t subAddress) {
	uint8_t data; // `data` will store the register data
	Wire.beginTransmission(address);         // Initialize the Tx buffer
	Wire.write(subAddress);	          // Put slave register address in Tx buffer
	Wire.endTransmission(I2C_NOSTOP); // Send the Tx buffer, but send a restart to keep connection alive
//	Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
//	Wire.requestFrom(address, 1);  // Read one byte from slave register address
	Wire.requestFrom(address, (size_t) 1); // Read one byte from slave register address
	data = Wire.read();                      // Fill Rx buffer with result
	return data;                         // Return data read from slave register
}

void writeByte(uint8_t address, uint8_t subAddress, uint8_t data) {
	Wire.beginTransmission(address);  // Initialize the Tx buffer
	Wire.write(subAddress);           // Put slave register address in Tx buffer
	Wire.write(data);                 // Put data in Tx buffer
	Wire.endTransmission();           // Send the Tx buffer
}

void M24512DFMreadBytes(uint8_t device_address, uint8_t data_address1,
		uint8_t data_address2, uint8_t count, uint8_t * dest) {
	Wire.beginTransmission(device_address);   // Initialize the Tx buffer
	Wire.write(data_address1);        // Put slave register address in Tx buffer
	Wire.write(data_address2);        // Put slave register address in Tx buffer
	Wire.endTransmission(I2C_NOSTOP); // Send the Tx buffer, but send a restart to keep connection alive
//	Wire.endTransmission(false);              // Send the Tx buffer, but send a restart to keep connection alive
	uint8_t i = 0;
//        Wire.requestFrom(address, count);       // Read bytes from slave register address
	Wire.requestFrom(device_address, (size_t) count); // Read bytes from slave register address
	while (Wire.available()) {
		dest[i++] = Wire.read();
	}                // Put read results in the Rx buffer
}

