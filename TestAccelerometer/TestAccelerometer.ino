#include "Algorithm.h"
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
BMP280 bmp;
EEPROM eeprom;
EM7180 em7180;
SENtral sentral;

void setup() {
	// setup for Master mode, pins 18/19, external pullups, 400kHz for Teensy 3.1
	Wire.begin(I2C_MASTER, 0x00, I2C_PINS_16_17, I2C_PULLUP_EXT, I2C_RATE_400);
	delay(5000);
	Serial.begin(115200);

	// Set up the interrupt pin, its set as active high, push-pull
	//Ground
	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);

	//3V3
	pinMode(14, OUTPUT);
	digitalWrite(14, HIGH);

	//Should detect the SENtral at 0x28
	//Scan must be perfomed for some reason.
	ReadWriteByte::I2Cscan();

	sentral.readSENtralDevices();

	em7180.SensorsDetected();

	eeprom.initEEPROM();
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

	Serial.println("GO!!!!");
}


uint8_t printOnce = 1;
uint8_t oldX = 0.0f, oldY = 0.0f, oldZ = 0.0f;

uint16_t count = 0;
// the loop function runs over and over again until power down or reset
void loop() {
	float ax[200], ay[200], az[200];
	float gx[200], gy[200], gz[200];

	ArrayList<float> inputArray;
	ArrayList<float> outputArray;

	for (int i = 0; i < 5; i++) {

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

		ax[i] = 1000 * accelX;
		ay[i] = 1000 * accelY;
		az[i] = 1000 * accelZ;

		gx[i] = gyroX;
		gy[i] = gyroY;
		gz[i] = gyroZ;

		inputArray.add(Algorithm::magnitudeVector(ax[i], ay[i], az[i]));
		delay(50);
	}
//Idee van wilbert, werkte niet.
//	int mean = Algorithm::mean(inputArray, inputArray.getSize());
//	Serial.print(mean);
//	Serial.print('\t');
//	if(mean > 1050){
//		Serial.println(1);
//	}
//	else{
//		Serial.println(0);
//	}

//Orgineel iedee, hebben peak detection beetje aangepsast.
	//Algorithm::movingAverage(inputArray, outputArray, 5);
	//ArrayList<Algorithm::Peak> peaks;
	//Algorithm::peakDetection(outputArray, peaks, 1050, 950);

//	Serial.println("---------???------------------");
//	for (int i = 0; i < outputArray.getSize(); i++) {
//		Serial.println(outputArray[i]);
//
//	}
//		if (printOnce == 1) {
//			Serial.println("aX \t aY \t aZ \t gX \t gY \t gZ \t mA accel \t peak pos \t peak");
//			printOnce++;
//		}
//		Serial.print(ax[i]);
//		Serial.print("\t");
//		Serial.print(ay[i]);
//		Serial.print("\t");
//		Serial.print(az[i]);
//		Serial.print("\t");
//		Serial.print(gx[i], 2);
//		Serial.print("\t");
//		Serial.print(gy[i], 2);
//		Serial.print("\t");
//		Serial.print(gz[i], 2);
//		Serial.print("\t");
//		Serial.print(outputArray[i]);
//		Serial.print("\t");
//		Serial.print(peaks[i].position);
//		Serial.print("\t");
//		Serial.print(peaks[i].peak);
//		Serial.print("\n");
//	}


	//while (true);
	//Serial.println((int)count);
}
