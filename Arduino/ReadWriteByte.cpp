/**
 * @file ReadWriteByte.cpp
 *
 */

#include "ReadWriteByte.h"
#include <Arduino.h>
#include <i2c_t3.h>

void ReadWriteByte::writeByte(uint8_t address, uint8_t subAddress, uint8_t data){
	Wire.beginTransmission(address);  // Initialize the Tx buffer
	Wire.write(subAddress);           // Put slave register address in Tx buffer
	Wire.write(data);                 // Put data in Tx buffer
	Wire.endTransmission();           // Send the Tx buffer
}

bool ReadWriteByte::writeRegister(const uint8_t register_addr, const uint8_t mpuDefaultRegister, const uint8_t value) {
	//send write call to sensor address
	//send register address to sensor
	//send value to register
	bool write_status = 0;
	Wire.beginTransmission(mpuDefaultRegister); //open communication with 
	Wire.write(register_addr);
	Wire.write(value);
	Wire.endTransmission();

	return write_status; //returns whether the write succeeded or failed
}

bool ReadWriteByte::writeMaskedRegister(const uint8_t register_addr, const uint8_t mpuDefaultRegister, const uint8_t mask, const uint8_t value) {
	uint8_t masked_value = (mask & value);
	uint8_t regvalue = readRegister(register_addr, mpuDefaultRegister);
	regvalue = regvalue & ~mask; // Zero Mask bits
	regvalue = regvalue | masked_value; //Set Mask value
	return writeRegister(register_addr, mpuDefaultRegister, regvalue);
}

uint8_t ReadWriteByte::readRegister(const uint8_t register_addr, const uint8_t mpuDefaultRegister) {
	//call sensor by address
	//call registers
	uint8_t data = 0;

	Wire.beginTransmission(mpuDefaultRegister);
	Wire.write(register_addr);
	Wire.endTransmission();

	Wire.requestFrom((int)mpuDefaultRegister, 1);

	while (Wire.available()) {
		data = Wire.read();    // receive a byte as character
	}

	return data; //return the data returned from the register
}

void ReadWriteByte::M24512DFMreadBytes(uint8_t device_address, uint8_t data_address1, uint8_t data_address2, uint8_t count, uint8_t * dest)
{
	Wire.beginTransmission(device_address);   // Initialize the Tx buffer
	Wire.write(data_address1);                     // Put slave register address in Tx buffer
	Wire.write(data_address2);                     // Put slave register address in Tx buffer
	Wire.endTransmission(I2C_NOSTOP);         // Send the Tx buffer, but send a restart to keep connection alive
											  //	Wire.endTransmission(false);              // Send the Tx buffer, but send a restart to keep connection alive
	uint8_t i = 0;
	//        Wire.requestFrom(address, count);       // Read bytes from slave register address 
	Wire.requestFrom(device_address, (size_t)count);  // Read bytes from slave register address 
	while (Wire.available()) {
		dest[i++] = Wire.read();
	}                // Put read results in the Rx buffer
}

uint8_t ReadWriteByte::readByte(uint8_t address, uint8_t subAddress){
	uint8_t data; // `data` will store the register data	 
	Wire.beginTransmission(address);         // Initialize the Tx buffer
	Wire.write(subAddress);	                 // Put slave register address in Tx buffer
	Wire.endTransmission(I2C_NOSTOP);        // Send the Tx buffer, but send a restart to keep connection alive
											 //	Wire.endTransmission(false);             // Send the Tx buffer, but send a restart to keep connection alive
											 //	Wire.requestFrom(address, 1);  // Read one byte from slave register address 
	Wire.requestFrom(address, (size_t)1);   // Read one byte from slave register address 
	data = Wire.read();                      // Fill Rx buffer with result
	return data;                             // Return data read from slave register
}

void ReadWriteByte::readBytes(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * dest){
	Wire.beginTransmission(address);   // Initialize the Tx buffer
	Wire.write(subAddress);            // Put slave register address in Tx buffer
	Wire.endTransmission(I2C_NOSTOP);  // Send the Tx buffer, but send a restart to keep connection alive
									   //	Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
	uint8_t i = 0;
	//        Wire.requestFrom(address, count);  // Read bytes from slave register address 
	Wire.requestFrom(address, (size_t)count);  // Read bytes from slave register address 
	while (Wire.available()) {
		dest[i++] = Wire.read();
	}         // Put read results in the Rx buffer
}

bool ReadWriteByte::I2Cscan(){
	int nDevices = 0;
	for (byte address = 1; address < 127; address++) {
		// The i2c_scanner uses the return value of
		// the Write.endTransmisstion to see if
		// a device did acknowledge to the address.
		Wire.beginTransmission(address);
		delay(1);
		byte error = Wire.endTransmission(I2C_STOP, 50);
		if (error == 0) {
			nDevices++;
		}
	}
	return nDevices > 0;
}
