#include "BMP280.h"
#include "DefineAddress.h"
#include "EEPROM.h"
#include "EM7180.h"
#include "SENtral.h"
#include <i2c_t3.h>
#include "ReadWriteByte.h"

bool passThru = false;

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

//creating the objects
BMP280 BMP;
EEPROM eeprom;
EM7180 em7180;
SENtral sentral;

void setup() {
	// setup for Master mode, pins 18/19, external pullups, 400kHz for Teensy 3.1
	Wire.begin(I2C_MASTER, 0x00, I2C_PINS_16_17, I2C_PULLUP_EXT, I2C_RATE_400);
	delay(5000);
	Serial.begin(38400);

	// Set up the interrupt pin, its set as active high, push-pull
	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);

	//Should detect the SENtral at 0x28
	ReadWriteByte::I2Cscan();

	sentral.readSENtralDevices();

	em7180.SensorsDetected();

	eeprom.initEEPROM();

	if (!passThru) {
		em7180.EM7180InitState();
		em7180.setupSensors();
		em7180.configureOM();

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

		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_ParamRequest, 0x00); //End parameter transfer
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x00); // re-enable algorithm

		em7180.EM7180Status();
		em7180.sensorState();
	}
	else if (passThru) {
		sentral.SENtralPassThroughMode();
		//should see all the devices on the I2C bus including two from the EEPROM
		ReadWriteByte::I2Cscan();

		// Read first page of EEPROM
		uint8_t data[128];
		ReadWriteByte::M24512DFMreadBytes(M24512DFM_DATA_ADDRESS, 0x00, 0x00, 128, data);
		Serial.println("EEPROM Signature Byte");
		Serial.print(data[0], HEX); Serial.println("  Should be 0x2A");
		Serial.print(data[1], HEX); Serial.println("  Should be 0x65");
		for (int i = 0; i < 128; i++) {
			Serial.print(data[i], HEX); Serial.print(" ");
		}

		// Set up the interrupt pin, its set as active high, push-pull
		pinMode(13, OUTPUT);
		digitalWrite(13, HIGH);

		// Read the WHO_AM_I register, this is a good test of communication
		Serial.println("MPU9250 9-axis motion sensor...");
		byte c = ReadWriteByte::readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);  // Read WHO_AM_I register for MPU-9250
		if (c != 0x71) {
			Serial.print("MPU9250 "); Serial.print("I AM "); Serial.print(c, HEX); Serial.print(" I should be "); Serial.println(0x71, HEX);
		}
		else if (c == 0x71) // WHO_AM_I should always be 0x71
		{
			Serial.println("MPU9250 is online...");

			float SelfTest[6];            // holds results of gyro and accelerometer self test
			eeprom.MPU9250SelfTest(SelfTest); // Start by performing self test and reporting values

			// get sensor resolutions, only need to do this oncea 
			eeprom.getAccelResult();
			eeprom.getGyroResult();
			eeprom.getMagResult();

			Serial.println(" Calibrate gyro and accel");
			eeprom.accelGyroCalMPU9250(gyroBias, accelBias); // Calibrate gyro and accelerometers, load biases in bias registers
			Serial.println("accel biases (mg)"); Serial.println(1000.*accelBias[0]); Serial.println(1000.*accelBias[1]); Serial.println(1000.*accelBias[2]);
			Serial.println("gyro biases (dps)"); Serial.println(gyroBias[0]); Serial.println(gyroBias[1]); Serial.println(gyroBias[2]);

			delay(1000);

			eeprom.initMPU9250();
			Serial.println("MPU9250 initialized for active data mode...."); // Initialize device for active mode read of acclerometer, gyroscope, and temperature
			ReadWriteByte::writeByte(MPU9250_ADDRESS, INT_PIN_CFG, 0x22);
			ReadWriteByte::I2Cscan(); // should see all the devices on the I2C bus including two from the EEPROM (ID page and data pages)

				   // Read the WHO_AM_I register of the magnetometer, this is a good test of communication
			byte d = ReadWriteByte::readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);  // Read WHO_AM_I register for AK8963
			Serial.print("AK8963 "); Serial.print("I AM "); Serial.print(d, HEX); Serial.print(" I should be "); Serial.println(0x48, HEX);

			delay(1000);

			// Get magnetometer calibration from AK8963 ROM
			eeprom.initAK8963(eeprom.magCalibration); Serial.println("AK8963 initialized for active data mode...."); // Initialize device for active mode read of magnetometer

			eeprom.magcalMPU9250(magBias, magScale);
			Serial.println("AK8963 mag biases (mG)"); Serial.println(magBias[0]); Serial.println(magBias[1]); Serial.println(magBias[2]);
			Serial.println("AK8963 mag scale (mG)"); Serial.println(magScale[0]); Serial.println(magScale[1]); Serial.println(magScale[2]);
			delay(2000); // add delay to see results before serial spew of data

			if (SerialDebug) {
				//  Serial.println("Calibration values: ");
				Serial.print("sensitivity adjustment value");
				Serial.print("X-Axis "); Serial.print(eeprom.magCalibration[0], 2);
				Serial.print("Y-Axis "); Serial.print(eeprom.magCalibration[1], 2);
				Serial.print("Z-Axis "); Serial.print(eeprom.magCalibration[2], 2);
			}

			delay(1000);

			// Read the WHO_AM_I register of the BMP280 this is a good test of communication
			byte f = ReadWriteByte::readByte(BMP280_ADDRESS, BMP280_ID);  // Read WHO_AM_I register for BMP280
			Serial.print("BMP280 I am "); Serial.print(f, HEX);
			Serial.print(" I should be "); Serial.print(0x58, HEX);

			delay(1000);

			ReadWriteByte::writeByte(BMP280_ADDRESS, BMP280_RESET, 0xB6); // reset BMP280 before initilization
			delay(100);

			BMP.BMP280Init();
		}
		else
		{
			Serial.print("Could not connect to MPU9250: 0x");
			Serial.println(c, HEX);
			while (1); // Loop forever if communication doesn't happen
		}
	}
}

// the loop function runs over and over again until power down or reset
void loop() {
	int16_t rawPressure = 0, rawTemperature = 0;   // pressure, temperature raw count output
	float pressure = 0.0f;
	float temperature = 0.0f;
	if (!passThru) {

		// Check event status register, way to check data ready by polling rather than interrupt
		uint8_t eventStatus = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_EventStatus); // reading clears the register

		// Check for errors
		if (eventStatus & 0x02) { // error detected, what is it?

			uint8_t errorStatus = ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_ErrorRegister);
			if (errorStatus != 0x00) { // non-zero value indicates error, what is it?
				Serial.print(" EM7180 sensor status = "); Serial.println(errorStatus);
				if (errorStatus == 0x11) Serial.print("Magnetometer failure!");
				if (errorStatus == 0x12) Serial.print("Accelerometer failure!");
				if (errorStatus == 0x14) Serial.print("Gyro failure!");
				if (errorStatus == 0x21) Serial.print("Magnetometer initialization failure!");
				if (errorStatus == 0x22) Serial.print("Accelerometer initialization failure!");
				if (errorStatus == 0x24) Serial.print("Gyro initialization failure!");
				if (errorStatus == 0x30) Serial.print("Math error!");
				if (errorStatus == 0x80) Serial.print("Invalid sample rate!");
			}

			// Handle errors ToDo

		}

		// if no errors, see if new data is ready
		if (eventStatus & 0x10) { // new acceleration data available
			sentral.readSENtralData(accelCount, EM7180_AX);

			// Now we'll calculate the accleration value into actual g's
			accelX = (float)accelCount[0] * 0.000488;  // get actual g value
			accelY = (float)accelCount[1] * 0.000488;
			accelZ = (float)accelCount[2] * 0.000488;
		}

		if (eventStatus & 0x20) { // new gyro data available
			sentral.readSENtralData(gyroCount, EM7180_GX);

			// Now we'll calculate the gyro value into actual dps's
			gyroX = (float)gyroCount[0] * 0.153;  // get actual dps value
			gyroY = (float)gyroCount[1] * 0.153;
			gyroZ = (float)gyroCount[2] * 0.153;
		}

		if (eventStatus & 0x08) { // new mag data available
			sentral.readSENtralData(magCount, EM7180_MX);

			// Now we'll calculate the mag value into actual G's
			magX = (float)magCount[0] * 0.305176;  // get actual G value
			magY = (float)magCount[1] * 0.305176;
			magZ = (float)magCount[2] * 0.305176;
		}

		if (eventStatus & 0x04) { // new quaternion data available
			sentral.readSENtralQuatData(Quat);
		}

		// get BMP280 pressure
		if (eventStatus & 0x40) { // new baro data available
		  //   Serial.println("new Baro data!");
			rawPressure = sentral.readSENtralData(EM7180_Baro);
			pressure = (float)rawPressure*0.01f + 1013.25f; // pressure in mBar

			// get BMP280 temperature
			rawTemperature = sentral.readSENtralData(EM7180_Temp);
			temperature = (float)rawTemperature*0.01;  // temperature in degrees C
		}
	}

	else if (passThru) {
		// If intPin goes high, all data registers have new data
		//  if (digitalRead(intACC2)) {  // On interrupt, read data
		eeprom.readData(accelCount, ACCEL_XOUT_H);  // Read the x/y/z adc values

					  // Now we'll calculate the acceleration value into actual g's
		accelX = (float)accelCount[0] * eeprom.aRes - accelBias[0];  // get actual g value, this depends on scale being set
		accelY = (float)accelCount[1] * eeprom.aRes - accelBias[1];
		accelZ = (float)accelCount[2] * eeprom.aRes - accelBias[2];
		// } 
		//  if (digitalRead(intGYRO2)) {  // On interrupt, read data
		eeprom.readData(gyroCount, GYRO_XOUT_H);  // Read the x/y/z adc values

					  // Calculate the gyro value into actual degrees per second
		gyroX = (float)gyroCount[0] * eeprom.gRes;  // get actual gyro value, this depends on scale being set
		gyroY = (float)gyroCount[1] * eeprom.gRes;
		gyroZ = (float)gyroCount[2] * eeprom.gRes;
		// }
		//  if (digitalRead(intDRDYM)) {  // On interrupt, read data
		eeprom.readMagData(magCount);  // Read the x/y/z adc values

					// Calculate the magnetometer values in milliGauss
		magX = (float)magCount[0] * eeprom.mRes* eeprom.magCalibration[0] - magBias[0];  // get actual magnetometer value, this depends on scale being set
		magY = (float)magCount[1] * eeprom.mRes*eeprom.magCalibration[1] - magBias[1];
		magZ = (float)magCount[2] * eeprom.mRes*eeprom.magCalibration[2] - magBias[2];
		//    magX *= magScale[0];
		//    magY *= magScale[1];
		//    magZ *= magScale[2]; 
		//   }
	}


	// keep track of rates
	uint32_t count = 0, sumCount = 0; // used to control display output rate
	uint32_t Now = micros();
	float deltat = ((Now - lastUpdate) / 1000000.0f); // set integration time by time elapsed since last filter update
	lastUpdate = Now;

	sum += deltat; // sum for averaging filter update rate
	sumCount++;

	// Sensors x (y)-axis of the accelerometer/gyro is aligned with the y (x)-axis of the magnetometer;
	// the magnetometer z-axis (+ down) is misaligned with z-axis (+ up) of accelerometer and gyro!
	// We have to make some allowance for this orientation mismatch in feeding the output to the quaternion filter.
	// We will assume that +y accel/gyro is North, then x accel/gyro is East. So if we want te quaternions properly aligned
	// we need to feed into the madgwick function Ay, Ax, -Az, Gy, Gx, -Gz, Mx, My, and Mz. But because gravity is by convention
	// positive down, we need to invert the accel data, so we pass -Ay, -Ax, Az, Gy, Gx, -Gz, Mx, My, and Mz into the Madgwick
	// function to get North along the accel +y-axis, East along the accel +x-axis, and Down along the accel -z-axis.
	// This orientation choice can be modified to allow any convenient (non-NED) orientation convention.
	// This is ok by aircraft orientation standards!  
	// Pass gyro rate as rad/s
	eeprom.MadgwickQuaternionUpdate(-accelY, -accelX, accelZ, gyroY*PI / 180.0f, gyroX*PI / 180.0f, -gyroZ*PI / 180.0f, magX, magY, magZ, deltat);
	//  if(passThru)MahonyQuaternionUpdate(-accelY, -accelX, accelZ, gyroY*PI/180.0f, gyroX*PI/180.0f, -gyroZ*PI/180.0f,  magX,  magY, magZ);

	// Serial print and/or display at 0.5 s rate independent of data rates
	uint32_t delt_t = millis() - count;
	if (delt_t > 500) { // update LCD once per half-second independent of read rate

		if (SerialDebug) {
			Serial.print("accelX = "); Serial.print((int)1000 * accelX);
			Serial.print(" accelY = "); Serial.print((int)1000 * accelY);
			Serial.print(" accelZ = "); Serial.print((int)1000 * accelZ); Serial.println(" mg");
			Serial.print("gyroX = "); Serial.print(gyroX, 2);
			Serial.print(" gyroY = "); Serial.print(gyroY, 2);
			Serial.print(" gyroZ = "); Serial.print(gyroZ, 2); Serial.println(" deg/s");
			Serial.print("magX = "); Serial.print((int)magX);
			Serial.print(" magY = "); Serial.print((int)magY);
			Serial.print(" magZ = "); Serial.print((int)magZ); Serial.println(" mG");

			Serial.println("Software quaternions:");
			Serial.print("q0 = "); Serial.print(eeprom.q[0]);
			Serial.print(" qx = "); Serial.print(eeprom.q[1]);
			Serial.print(" qy = "); Serial.print(eeprom.q[2]);
			Serial.print(" qz = "); Serial.println(eeprom.q[3]);
			Serial.println("Hardware quaternions:");
			Serial.print("Q0 = "); Serial.print(Quat[0]);
			Serial.print(" Qx = "); Serial.print(Quat[1]);
			Serial.print(" Qy = "); Serial.print(Quat[2]);
			Serial.print(" Qz = "); Serial.println(Quat[3]);
		}
		if (passThru) {
			int32_t rawPress = BMP.readBMP280Pressure();
			pressure = (float)BMP.BMP280_compensate_P(rawPress) / 25600.; // Pressure in mbar
			int32_t rawTemp = BMP.readBMP280Temperature();
			temperature = (float)BMP.BMP280_compensate_T(rawTemp) / 100.;

		}


		// Define output variables from updated quaternion---these are Tait-Bryan angles, commonly used in aircraft orientation.
		// In this coordinate system, the positive z-axis is down toward Earth. 
		// Yaw is the angle between Sensor x-axis and Earth magnetic North (or true North if corrected for local declination, looking down on the sensor positive yaw is counterclockwise.
		// Pitch is angle between sensor x-axis and Earth ground plane, toward the Earth is positive, up toward the sky is negative.
		// Roll is angle between sensor y-axis and Earth ground plane, y-axis up is positive roll.
		// These arise from the definition of the homogeneous rotation matrix constructed from quaternions.
		// Tait-Bryan angles as well as Euler angles are non-commutative; that is, the get the correct orientation the rotations must be
		// applied in the correct order which for this configuration is yaw, pitch, and then roll.
		// For more see http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles which has additional links.
		//Software AHRS:
		float softwareYaw = eeprom.calculateSoftwareYPR("Y");
		float softwarePitch = eeprom.calculateSoftwareYPR("P");
		float softwareRoll = eeprom.calculateSoftwareYPR("R");

		softwareYaw *= 180.0f / PI;
		softwareYaw += 13.8f; // Declination at Danville, California is 13 degrees 48 minutes and 47 seconds on 2014-04-04

		if (softwareYaw < 0) {
			softwareYaw += 360.0f; // Ensure yaw stays between 0 and 360
		}
		softwarePitch *= 180.0f / PI;
		softwareRoll *= 180.0f / PI;

		//Hardware AHRS:
		float hardwareYaw = atan2(2.0f * (Quat[0] * Quat[1] + Quat[3] * Quat[2]), Quat[3] * Quat[3] + Quat[0] * Quat[0] - Quat[1] * Quat[1] - Quat[2] * Quat[2]);
		float hardwarePitch = -asin(2.0f * (Quat[0] * Quat[2] - Quat[3] * Quat[1]));
		float hardwareRoll = atan2(2.0f * (Quat[3] * Quat[0] + Quat[1] * Quat[2]), Quat[3] * Quat[3] - Quat[0] * Quat[0] - Quat[1] * Quat[1] + Quat[2] * Quat[2]);
		hardwareYaw *= 180.0f / PI;
		hardwareYaw += 13.8f; // Declination at Danville, California is 13 degrees 48 minutes and 47 seconds on 2014-04-04

		if (hardwareYaw < 0) {
			hardwareYaw += 360.0f; // Ensure yaw stays between 0 and 360
		}
		hardwarePitch *= 180.0f / PI;
		hardwareRoll *= 180.0f / PI;

		// Or define output variable according to the Android system, where heading (0 to 360) is defined by the angle between the y-axis 
		// and True North, pitch is rotation about the x-axis (-180 to +180), and roll is rotation about the y-axis (-90 to +90)
		// In this systen, the z-axis is pointing away from Earth, the +y-axis is at the "top" of the device (cellphone) and the +x-axis
		// points toward the right of the device.
		//

		if (SerialDebug) {
			Serial.print("Software yaw, pitch, roll: ");
			Serial.print(softwareYaw, 2);
			Serial.print(", ");
			Serial.print(softwarePitch, 2);
			Serial.print(", ");
			Serial.println(softwareRoll, 2);

			Serial.print("Hardware Yaw, Pitch, Roll: ");
			Serial.print(hardwareYaw, 2);
			Serial.print(", ");
			Serial.print(hardwarePitch, 2);
			Serial.print(", ");
			Serial.println(hardwareRoll, 2);


			Serial.println("BMP280:");
			Serial.print("Altimeter temperature = ");
			Serial.print(temperature, 2);
			Serial.println(" C"); // temperature in degrees Celsius
			Serial.print("Altimeter temperature = ");
			Serial.print(9.*temperature / 5. + 32., 2);
			Serial.println(" F"); // temperature in degrees Fahrenheit
			Serial.print("Altimeter pressure = ");
			Serial.print(pressure, 2);
			Serial.println(" mbar");// pressure in millibar
			float altitude = 145366.45f*(1.0f - pow((pressure / 1013.25f), 0.190284f));
			Serial.print("Altitude = ");
			Serial.print(altitude, 2);
			Serial.println(" feet");
			Serial.println(" ");
		}

		Serial.print("rate = "); Serial.print((float)sumCount / sum, 2); Serial.println(" Hz");
		//     Serial.print(millis()/1000.0, 1);Serial.print(",");
		//     Serial.print(yaw); Serial.print(",");Serial.print(pitch); Serial.print(",");Serial.print(roll); Serial.print(",");
		//     Serial.print(Yaw); Serial.print(",");Serial.print(Pitch); Serial.print(",");Serial.println(Roll);  

		int led = 13;
		digitalWrite(led, !digitalRead(led));

		// tempCount = readTempData();  // Read the gyro adc values
		//    temperature = ((float) tempCount) / 333.87 + 21.0; // Gyro chip temperature in degrees Centigrade
		// Print temperature in degrees Centigrade      
		//    Serial.print("Gyro temperature is ");  Serial.print(temperature, 1);  Serial.println(" degrees C"); // Print T values to tenths of s degree C
	}
}

