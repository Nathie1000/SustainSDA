// 
// 
// 

#include "SENtral.h"

	SENtral::SENtral() {

	}

	SENtral::~SENtral() {

	}
	// Read SENtral device information
	void SENtral::readSENtralDevices() {
		uint16_t ROM1 = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_ROMVersion1);
		uint16_t ROM2 = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_ROMVersion2);
		Serial.print("EM7180 ROM Version: 0x"); Serial.print(ROM1, HEX); Serial.println(ROM2, HEX); Serial.println("Should be: 0xE609");
		uint16_t RAM1 = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_RAMVersion1);
		uint16_t RAM2 = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_RAMVersion2);
		Serial.print("EM7180 RAM Version: 0x"); Serial.print(RAM1); Serial.println(RAM2);
		uint8_t ProductID = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_ProductID);
		Serial.print("EM7180 ProductID: 0x"); Serial.print(ProductID, HEX); Serial.println(" Should be: 0x80");
		uint8_t RevisionID = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_RevisionID);
		Serial.print("EM7180 RevisionID: 0x"); Serial.print(RevisionID, HEX); Serial.println(" Should be: 0x02");

		delay(1000); // give some time to read the screen
	}

	void SENtral::SENtralPassThroughMode() {
		// First put SENtral in standby mode
		uint8_t c = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_AlgorithmControl);
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, c | 0x01);
		Serial.println("SENtral in standby mode");

		// Place SENtral in pass-through mode
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_PassThruControl, 0x01);
		if (ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_PassThruStatus) & 0x01) {
			Serial.println("SENtral in pass-through mode");
			delay(100);
		}
		else {
			Serial.println("ERROR! SENtral not in pass-through mode!");
			delay(100);
		}
	}

	float SENtral::uint32_reg_to_float(uint8_t *buf)
	{
		union {
			uint32_t ui32;
			float f;
		} u;

		u.ui32 = (((uint32_t)buf[0]) +
			(((uint32_t)buf[1]) << 8) +
			(((uint32_t)buf[2]) << 16) +
			(((uint32_t)buf[3]) << 24));
		return u.f;
	}

	void SENtral::readSENtralData(int16_t * destination, int deviceAddress) {
		uint8_t rawData[6];  // x/y/z accel register data stored here
		ReadWriteByte::readBytes(EM7180_ADDRESS, deviceAddress, 6, &rawData[0]);       // Read the six raw data registers into data array
		destination[0] = (int16_t)(((int16_t)rawData[1] << 8) | rawData[0]);  // Turn the MSB and LSB into a signed 16-bit value
		destination[1] = (int16_t)(((int16_t)rawData[3] << 8) | rawData[2]);
		destination[2] = (int16_t)(((int16_t)rawData[5] << 8) | rawData[4]);
	}

	void SENtral::readSENtralQuatData(float * destination)
	{
		uint8_t rawData[16];  // x/y/z quaternion register data stored here
		ReadWriteByte::readBytes(EM7180_ADDRESS, EM7180_QX, 16, &rawData[0]);       // Read the sixteen raw data registers into data array
		destination[0] = uint32_reg_to_float(&rawData[0]);
		destination[1] = uint32_reg_to_float(&rawData[4]);
		destination[2] = uint32_reg_to_float(&rawData[8]);
		destination[3] = uint32_reg_to_float(&rawData[12]);  // SENtral stores quats as qx, qy, qz, q0!

	}

	int16_t SENtral::readSENtralData(int deviceAddress) {
		uint8_t rawData[2];  // x/y/z gyro register data stored here
		ReadWriteByte::readBytes(EM7180_ADDRESS, deviceAddress, 2, &rawData[0]);  // Read the two raw data registers sequentially into data array
		return  (int16_t)(((int16_t)rawData[1] << 8) | rawData[0]);   // Turn the MSB and LSB into a signed 16-bit value
	}
