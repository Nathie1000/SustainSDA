// 
// 
// 

#include "EM7180.h"

	EM7180::EM7180() {

	}	
	
	EM7180::~EM7180() {

	}

	//Sensors that are detected by the EM7180
	void EM7180::SensorsDetected() {
		uint8_t featureflag = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_FeatureFlags);
		if (featureflag & 0x01)  Serial.println("A barometer is installed");
		if (featureflag & 0x02)  Serial.println("A humidity sensor is installed");
		if (featureflag & 0x04)  Serial.println("A temperature sensor is installed");
		if (featureflag & 0x08)  Serial.println("A custom sensor is installed");
		if (featureflag & 0x10)  Serial.println("A second custom sensor is installed");
		if (featureflag & 0x20)  Serial.println("A third custom sensor is installed");

		delay(1000); // give some time to read the screen
	}

	void EM7180::EM7180InitState() {
		// Enter EM7180 initialized state
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_HostControl, 0x00); // set SENtral in initialized state to configure registers
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_PassThruControl, 0x00); // make sure pass through mode is off
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_HostControl, 0x01); // Force initialize
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_HostControl, 0x00); // set SENtral in initialized state to configure registers
	}

	void EM7180::setupSensors() {
		//Setup LPF bandwidth (BEFORE setting ODR's)
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_ACC_LPF_BW, 0x03); // 41Hz
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_GYRO_LPF_BW, 0x03); // 41Hz
															 // Set accel/gyro/mage desired ODR rates
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_QRateDivisor, 0x02); // 100 Hz
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_MagRate, 0x64); // 100 Hz
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_AccelRate, 0x14); // 200/10 Hz
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_GyroRate, 0x14); // 200/10 Hz
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_BaroRate, 0x80 | 0x32);  // set enable bit and set Baro rate to 25 Hz
	}

	void EM7180::configureOM() {
		// Configure operating mode
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x00); // read scale sensor data
																  // Enable interrupt to host upon certain events
																  // choose host interrupts when any sensor updated (0x40), new gyro data (0x20), new accel data (0x10),
																  // new mag data (0x08), quaternions updated (0x04), an error occurs (0x02), or the SENtral needs to be reset(0x01)
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_EnableEvents, 0x07);
		// Enable EM7180 run mode
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_HostControl, 0x01); // set SENtral in normal run mode
		delay(100);

	}

	void EM7180::readSensorFS(uint16_t EM7180FS, int adress, int paramIndex) {
		uint8_t param[4];                         // used for param transfer
		byte param_xfer;

		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_ParamRequest, adress); // Request to read parameter 74
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x80); // Request parameter transfer process
		param_xfer = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge);
		while (!(param_xfer == adress)) {
			param_xfer = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge);
		}
		param[0] = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_SavedParamByte0);
		param[1] = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_SavedParamByte1);
		param[2] = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_SavedParamByte2);
		param[3] = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_SavedParamByte3);
		//test
		EM7180FS = ((int16_t)(param[paramIndex] << 8) | param[(paramIndex - 1)]);
		Serial.println(EM7180FS);
		delay(100);
		//
	}

	void EM7180::EM7180SetModuleAccFS(uint16_t moduleFS, uint16_t accelerometerFS, int adress) {
		uint8_t bytes[4], STAT;
		bytes[0] = moduleFS & (0xFF);
		bytes[1] = (moduleFS >> 8) & (0xFF);
		if (accelerometerFS == 0x00) {

			bytes[2] = accelerometerFS;
			bytes[3] = accelerometerFS;
		}
		else {
			bytes[2] = accelerometerFS & (0xFF);
			bytes[3] = (accelerometerFS >> 8) & (0xFF);
		}
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_LoadParamByte0, bytes[0]); //Mag LSB
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_LoadParamByte1, bytes[1]); //Mag MSB
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_LoadParamByte2, bytes[2]); //Acc LSB
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_LoadParamByte3, bytes[3]); //Acc MSB
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_ParamRequest, adress); //Parameter 74; 0xCA is 74 decimal with the MSB set high to indicate a paramter write processs
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x80); //Request parameter transfer procedure
		STAT = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge); //Check the parameter acknowledge register and loop until the result matches parameter request byte
		while (!(STAT == adress)) {
			STAT = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge);
		}
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_ParamRequest, 0x00); //Parameter request = 0 to end parameter transfer process
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x00); // Re-start algorithm
	}

	void EM7180::EM7180SetIntInParam(uint8_t param, uint32_t paramValue) {
		uint8_t bytes[4], STAT;
		bytes[0] = paramValue & (0xFF);
		bytes[1] = (paramValue >> 8) & (0xFF);
		bytes[2] = (paramValue >> 16) & (0xFF);
		bytes[3] = (paramValue >> 24) & (0xFF);
		param = param | 0x80; //Parameter is the decimal value with the MSB set high to indicate a paramter write processs
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_LoadParamByte0, bytes[0]); //Param LSB
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_LoadParamByte1, bytes[1]);
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_LoadParamByte2, bytes[2]);
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_LoadParamByte3, bytes[3]); //Param MSB
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_ParamRequest, param);
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x80); //Request parameter transfer procedure
		STAT = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge); //Check the parameter acknowledge register and loop until the result matches parameter request byte
		while (!(STAT == param)) {
			STAT = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge);
		}
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_ParamRequest, 0x00); //Parameter request = 0 to end parameter transfer process
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x00); // Re-start algorithm
	}

	void EM7180::EM7180Status() {
		uint8_t runStatus = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_RunStatus);
		if (runStatus & 0x01) Serial.println("EM7180 run status = normal mode");

		uint8_t algorithmStatus = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_AlgorithmStatus);
		switch (algorithmStatus) {
		case 0x01: Serial.println("EM7180 standby status");
		case 0x02: Serial.println("EM7180 algorithm slow");
		case 0x04: Serial.println("EM7180 in stillness mode");
		case 0x08: Serial.println("EM7180 mag calibration completed");
		case 0x10: Serial.println("EM7180 magnetic anomaly detected");
		case 0x20: Serial.println("EM7180 unreliable sensor data");
		}

		uint8_t passThruStatus = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_PassThruStatus);
		if (passThruStatus & 0x01) Serial.print(" EM7180 in passthru mode!");

		uint8_t eventStatus = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_EventStatus);
		switch (eventStatus) {
		case 0x01: Serial.println("EM7180 CPU reset");
		case 0x02: Serial.println("EM7180 Error");
		case 0x04: Serial.println("EM7180 new quaternion result");
		case 0x08: Serial.println("EM7180 new mag result");
		case 0x10: Serial.println("EM7180 new accel result");
		case 0x20: Serial.println("EM7180 new gyro result");
		}
		delay(200);
	}

	void EM7180::sensorState() {
		uint8_t sensorStatus = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_SensorStatus);
		//Serial.print(" EM7180 sensor status = "); Serial.println(sensorStatus);
		switch (sensorStatus) {
		case 0x01: Serial.print("Magnetometer not acknowledging!");
		case 0x02: Serial.print("Accelerometer not acknowledging!");
		case 0x04: Serial.print("Gyro not acknowledging!");
		case 0x08: Serial.print("Magnetometer ID not recognized!");
		case 0x10: Serial.print("Accelerometer ID not recognized!");
		case 0x20: Serial.print("Gyro ID not recognized!");
		}
	}
