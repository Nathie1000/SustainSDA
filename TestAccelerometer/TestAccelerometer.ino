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


//TestData stepdetection

//float ax[] = {
//985.27f,
//992.59f,
//1002.35f,
//991.13f,
//994.06f,
//988.69f,
//972.1f,
//993.57f,
//994.54f,
//1004.79f,
//1016.02f,
//971.61f,
//1049.69f,
//1045.78f,
//922.32f,
//935.98f,
//1037.49f,
//980.88f,
//1096.05f,
//1005.28f,
//1007.23f,
//987.71f,
//965.75f,
//963.8f,
//985.27f,
//1029.19f,
//996.01f,
//1030.17f,
//1111.66f,
//958.43f,
//853.51f,
//786.17f,
//1134.11f,
//956.48f,
//1119.96f,
//1078.48f,
//993.57f,
//946.23f,
//991.62f,
//967.22f,
//907.68f,
//1016.99f,
//975.02f,
//894.02f,
//829.6f,
//845.22f,
//899.38f,
//942.82f,
//905.24f,
//864.74f,
//818.86f,
//854.0f,
//1472.78f,
//1068.72f,
//1087.26f,
//1104.83f,
//867.66f,
//942.82f,
//982.34f,
//992.1f,
//932.08f,
//1014.06f,
//981.37f,
//1001.38f,
//955.5f,
//1000.89f,
//1173.15f,
//945.26f,
//1015.53f,
//1072.14f,
//1003.82f,
//886.7f,
//651.97f,
//836.92f,
//1183.89f,
//1247.82f,
//1030.66f,
//910.61f,
//803.74f,
//855.46f,
//953.06f,
//971.12f,
//988.69f,
//985.27f,
//968.19f,
//980.88f,
//981.86f,
//1116.54f,
//1088.24f,
//1137.04f,
//877.91f,
//945.26f,
//788.12f,
//867.66f,
//850.1f,
//893.04f,
//755.42f,
//748.59f,
//1508.41f,
//1108.25f,
//1228.3f,
//1059.94f,
//981.86f,
//935.98f,
//987.71f,
//1021.38f,
//1006.26f,
//991.13f,
//967.7f,
//1008.21f,
//1052.62f,
//909.63f,
//909.63f,
//1103.37f,
//1064.33f,
//992.59f,
//936.96f,
//903.29f,
//930.62f,
//1220.49f,
//926.22f,
//1094.1f,
//931.1f,
//850.58f,
//885.23f,
//1030.66f,
//1003.33f,
//1033.58f,
//1069.7f,
//1079.94f,
//1030.66f,
//1033.58f,
//945.26f,
//896.46f,
//894.5f,
//735.9f,
//888.16f,
//908.17f,
//771.53f,
//804.71f,
//956.97f,
//1439.11f,
//1127.77f,
//1088.73f,
//1069.7f,
//928.66f,
//914.02f,
//975.02f,
//998.94f,
//958.92f,
//1010.16f,
//947.7f,
//995.03f,
//1051.64f,
//975.51f,
//876.45f,
//1032.12f,
//1064.82f,
//1067.26f,
//837.9f,
//882.3f,
//906.7f,
//1436.18f,
//795.44f,
//1078.48f,
//800.81f,
//872.06f,
//902.31f,
//922.32f,
//993.08f,
//1063.35f,
//1181.45f,
//1142.41f,
//987.71f,
//929.64f,
//968.19f,
//851.56f,
//753.96f,
//834.48f,
//755.42f,
//908.17f,
//861.32f,
//834.48f,
//732.49f,
//1362.01f,
//991.13f,
//1194.14f,
//1048.22f,
//952.09f,
//990.64f,
//1015.53f,
//1057.98f
//};
//float ay[] = {
//	110.29f,
//	110.29f,
//	103.46f,
//	121.02f,
//	135.66f,
//	124.93f,
//	112.24f,
//	108.34f,
//	115.17f,
//	120.05f,
//	131.76f,
//	117.61f,
//	144.45f,
//	121.51f,
//	65.39f,
//	135.18f,
//	117.12f,
//	115.17f,
//	85.89f,
//	41.97f,
//	49.29f,
//	18.54f,
//	28.3f,
//	25.38f,
//	129.32f,
//	191.78f,
//	135.18f,
//	1.46f,
//	86.38f,
//	71.25f,
//	- 61.4f,
//	- 144.4f,
//	161.53f,
//	524.6f,
//	10.25f,
//	86.38f,
//	282.55f,
//	136.64f,
//	136.15f,
//	261.57f,
//	286.94f,
//	218.14f,
//	232.78f,
//	267.91f,
//	213.26f,
//	213.26f,
//	47.34f,
//	122.49f,
//	297.19f,
//	279.14f,
//	129.81f,
//	548.51f,
//	- 662.7f,
//	197.15f,
//	240.58f,
//	- 215.7f,
//	- 56.12f,
//	213.74f,
//	260.1f,
//	105.41f,
//	183.98f,
//	218.14f,
//	137.13f,
//	87.35f,
//	- 216.67f,
//	- 169.82f,
//	152.26f,
//	92.72f,
//	20.98f,
//	172.75f,
//	104.92f,
//	18.54f,
//	- 90.28f,
//	- 39.53f,
//	402.11f,
//	168.36f,
//	220.58f,
//	282.06f,
//	210.33f,
//	166.9f,
//	253.76f,
//	252.78f,
//	235.7f,
//	173.24f,
//	84.91f,
//	163.48f,
//	217.16f,
//	274.26f,
//	234.24f,
//	51.24f,
//	206.42f,
//	140.54f,
//	115.17f,
//	45.87f,
//	204.96f,
//	112.73f,
//	120.54f,
//	76.13f,
//	13.18f,
//	- 208.38f,
//	281.58f,
//	27.33f,
//	- 88.33f,
//	58.56f,
//	162.99f,
//	137.62f,
//	93.7f,
//	- 6.34f,
//	- 20.98f,
//	12.69f,
//	144.94f,
//	66.86f,
//	7.81f,
//	45.38f,
//	49.29f,
//	82.96f,
//	- 43.92f,
//	- 102.97f,
//	50.75f,
//	647.58f,
//	- 39.04f,
//	24.89f,
//	199.1f,
//	127.86f,
//	99.06f,
//	122.98f,
//	176.17f,
//	223.99f,
//	250.83f,
//	244.49f,
//	214.23f,
//	153.72f,
//	59.54f,
//	144.45f,
//	185.93f,
//	230.34f,
//	212.28f,
//	239.12f,
//	85.4f,
//	55.63f,
//	208.86f,
//	- 124.93f,
//	200.57f,
//	96.14f,
//	- 105.f,41
//	- 2.44f,
//	70.76f,
//	76.62f,
//	32.21f,
//	0.0f,
//	- 86.86f,
//	- 90.28f,
//	- 8.78f,
//	88.33f,
//	104.43f,
//	- 8.3f,
//	38.55f,
//	103.46f,
//	74.18f,
//	- 87.35f,
//	- 77.59f,
//	90.28f,
//	814.47f,
//	- 123.46f,
//	- 132.25f,
//	35.62f,
//	242.54f,
//	148.84f,
//	208.86f,
//	168.85f,
//	157.62f,
//	257.66f,
//	247.9f,
//	117.61f,
//	53.68f,
//	103.94f,
//	319.15f,
//	353.8f,
//	234.73f,
//	179.1f,
//	85.89f,
//	118.58f,
//	134.2f,
//	78.08f,
//	65.39f,
//	- 36.6f,
//	504.1f,
//	29.77f,
//	- 106.87f,
//	- 10.74f,
//	83.45f,
//	68.81f,
//};
//float az[] = {
//	87.35f,
//	92.23f,
//	101.99f,
//	99.55f,
//	95.65f,
//	97.11f,
//	93.7f,
//	77.1f,
//	81.5f,
//	81.98f,
//	68.81f,
//	50.26f,
//	23.91f,
//	52.22f,
//	82.96f,
//	77.1f,
//	100.04f,
//	83.45f,
//	106.38f,
//	83.45f,
//	60.51f,
//	62.95f,
//	39.53f,
//	61.98f,
//	139.08f,
//	129.81f,
//	105.41f,
//	71.25f,
//	52.22f,
//	78.57f,
//	24.89f,
//	- 11.22f,
//	252.3f,
//	138.1f,
//	55.63f,
//	152.74f,
//	238.63f,
//	236.68f,
//	226.92f,
//	238.14f,
//	212.28f,
//	232.78f,
//	234.73f,
//	265.96f,
//	224.48f,
//	219.6f,
//	227.9f,
//	158.6f,
//	161.53f,
//	169.82f,
//	17.57f,
//	413.34f,
//	219.11f,
//	224.97f,
//	183.0f,
//	154.7f,
//	210.82f,
//	114.19f,
//	148.35f,
//	103.46f,
//	134.2f,
//	177.63f,
//	107.85f,
//	24.4f,
//	102.97f,
//	223.5f,
//	143.96f,
//	193.74f,
//	131.76f,
//	148.35f,
//	130.78f,
//	54.66f,
//	35.14f,
//	368.44f,
//	197.15f,
//	119.07f,
//	272.3f,
//	207.89f,
//	243.02f,
//	236.68f,
//	192.76f,
//	181.54f,
//	116.14f,
//	130.78f,
//	42.46f,
//	23.91f,
//	115.17f,
//	151.77f,
//	318.66f,
//	260.59f,
//	224.48f,
//	139.57f,
//	251.81f,
//	255.71f,
//	225.46f,
//	254.74f,
//	109.31f,
//	53.68f,
//	155.67f,
//	135.18f,
//	192.76f,
//	185.44f,
//	164.94f,
//	200.57f,
//	94.18f,
//	98.09f,
//	98.58f,
//	109.31f,
//	135.66f,
//	141.52f,
//	138.59f,
//	97.6f,
//	73.2f,
//	- 5.37f,
//	37.58f,
//	20.01f,
//	- 6.83f,
//	- 20.98f,
//	311.83f,
//	183.49f,
//	141.52f,
//	183.0f,
//	201.06f,
//	208.86f,
//	121.51f,
//	111.75f,
//	98.09f,
//	57.58f,
//	72.22f,
//	102.48f,
//	201.54f,
//	302.56f,
//	265.96f,
//	290.36f,
//	271.33f,
//	262.54f,
//	229.85f,
//	148.35f,
//	185.93f,
//	100.04f,
//	280.11f,
//	124.44f,
//	258.64f,
//	238.14f,
//	194.71f,
//	173.24f,
//	203.5f,
//	204.47f,
//	222.53f,
//	223.5f,
//	181.54f,
//	179.1f,
//	151.77f,
//	154.7f,
//	147.38f,
//	103.46f,
//	77.1f,
//	74.66f,
//	46.36f,
//	41.97f,
//	38.55f,
//	14.64f,
//	277.18f,
//	345.02f,
//	271.82f,
//	323.54f,
//	302.56f,
//	189.34f,
//	180.56f,
//	228.38f,
//	172.26f,
//	183.0f,
//	285.97f,
//	394.79f,
//	333.79f,
//	406.5f,
//	284.99f,
//	284.5f,
//	233.26f,
//	224.97f,
//	142.01f,
//	140.54f,
//	36.11f,
//	3.9f,
//	377.22f,
//	146.89f,
//	124.93f,
//	200.08f,
//	226.92f,
//	156.16f,
//	202.03f,
//	197.64f
//};

void loop() {
	float ax[200], ay[200], az[200];
	//float gx[200], gy[200], gz[200];

	ArrayList<float> inputArray;
	ArrayList<float> outputArray;

	for (int i = 0; i < 40; i++) {

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

		//gx[i] = gyroX;
		//gy[i] = gyroY;
		//gz[i] = gyroZ;

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
	Algorithm::movingAverage(inputArray, outputArray, 5);
	ArrayList<Algorithm::Peak> peaks;
	Algorithm::peakDetection(outputArray, peaks, 1050, 1000);
	Algorithm::stepDetection(peaks, count);

	Serial.print("Steps: "); Serial.println(count);

	////obtaining test data
	//for (int i = 0; i < outputArray.getSize(); i++) {
	//	if (printOnce == 1) {
	//		Serial.println("aX \t aY \t aZ \t gX \t gY \t gZ \t mA accel \t peak pos \t peak");
	//		printOnce++;
	//	}
	//	Serial.print(ax[i]);
	//	Serial.print("\t");
	//	Serial.print(ay[i]);
	//	Serial.print("\t");
	//	Serial.print(az[i]);
	//	Serial.print("\t");
	//	Serial.print(gx[i], 2);
	//	Serial.print("\t");
	//	Serial.print(gy[i], 2);
	//	Serial.print("\t");
	//	Serial.print(gz[i], 2);
	//	Serial.print("\t");
	//	Serial.print(outputArray[i]);
	//	Serial.print("\t");
	//	Serial.print(peaks[i].position);
	//	Serial.print("\t");
	//	Serial.print(peaks[i].peak);
	//	Serial.print("\n");
	//}
	//while (true);
	////Testdata retrieved
}
