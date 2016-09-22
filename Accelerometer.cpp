// Do not remove the include below
#include "Accelerometer.h"

enum Ascale {
	AFS_2G = 0, AFS_4G, AFS_8G, AFS_16G
};

enum Gscale {
	GFS_250DPS = 0, GFS_500DPS, GFS_1000DPS, GFS_2000DPS
};

enum Mscale {
	MFS_14BITS = 0, // 0.6 mG per LSB
	MFS_16BITS      // 0.15 mG per LSB
};

float gyroBias[3] = { 0, 0, 0 }, accelBias[3] = { 0, 0, 0 }, magBias[3] = { 0,
		0, 0 }, magScale[3] = { 0, 0, 0 }; // Bias corrections for gyro, accelerometer, mag
float magCalibration[3] = { 0, 0, 0 }; // Factory mag calibration and mag bias
float SelfTest[6]; // holds results of gyro and accelerometer self test

// Specify sensor full scale
uint8_t Gscale = GFS_250DPS;
uint8_t Ascale = AFS_2G;
uint8_t Mscale = MFS_16BITS; // Choose either 14-bit or 16-bit magnetometer resolution
uint8_t Mmode = 0x02; // 2 for 8 Hz, 6 for 100 Hz continuous magnetometer data read
float aRes, gRes, mRes; // scale resolutions per LSB for the sensors

void I2Cscan() {
	// scan for i2c devices
	byte error, address;
	int nDevices;

	Serial.println("Scanning...");

	nDevices = 0;
	for (address = 1; address < 127; address++) {
		// The i2c_scanner uses the return value of
		// the Write.endTransmisstion to see if
		// a device did acknowledge to the address.
		Wire.beginTransmission(address);
		error = Wire.endTransmission();
		if (error == 0) {
			Serial.print("I2C device found at address 0x");
			if (address < 16)
				Serial.print("0");
			Serial.print(address, HEX);
			Serial.println("  !");

			nDevices++;
		} else if (error == 4) {
			Serial.print("Unknow error at address 0x");
			if (address < 16)
				Serial.print("0");
			Serial.println(address, HEX);
		}
	}
	if (nDevices == 0)
		Serial.println("No I2C devices found\n");
	else
		Serial.println("done\n");
}

// I2C read/write functions for the MPU9250 and AK8963 sensors

void SENtralPassThroughMode() {
	// First put SENtral in standby mode
	uint8_t c = readByte(EM7180_ADDRESS, EM7180_AlgorithmControl);
	writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, c | 0x01);
//  c = readByte(EM7180_ADDRESS, EM7180_AlgorithmStatus);
//  Serial.print("c = "); Serial.println(c);
// Verify standby status
// if(readByte(EM7180_ADDRESS, EM7180_AlgorithmStatus) & 0x01) {
	Serial.println("SENtral in standby mode");
	// Place SENtral in pass-through mode
	writeByte(EM7180_ADDRESS, EM7180_PassThruControl, 0x01);
	if (readByte(EM7180_ADDRESS, EM7180_PassThruStatus) & 0x01) {
		Serial.println("SENtral in pass-through mode");
	} else {
		Serial.println("ERROR! SENtral not in pass-through mode!");
	}

}


// Accelerometer and gyroscope self test; check calibration wrt factory settings

void getMres() {
	switch (Mscale) {
	// Possible magnetometer scales (and their register bit settings) are:
	// 14 bit resolution (0) and 16 bit resolution (1)
	case MFS_14BITS:
		mRes = 10. * 4912. / 8190.; // Proper scale to return milliGauss
		break;
	case MFS_16BITS:
		mRes = 10. * 4912. / 32760.0; // Proper scale to return milliGauss
		break;
	}
}

void getGres() {
	switch (Gscale) {
	// Possible gyro scales (and their register bit settings) are:
	// 250 DPS (00), 500 DPS (01), 1000 DPS (10), and 2000 DPS  (11).
	// Here's a bit of an algorith to calculate DPS/(ADC tick) based on that 2-bit value:
	case GFS_250DPS:
		gRes = 250.0 / 32768.0;
		break;
	case GFS_500DPS:
		gRes = 500.0 / 32768.0;
		break;
	case GFS_1000DPS:
		gRes = 1000.0 / 32768.0;
		break;
	case GFS_2000DPS:
		gRes = 2000.0 / 32768.0;
		break;
	}
}

void getAres() {
	switch (Ascale) {
	// Possible accelerometer scales (and their register bit settings) are:
	// 2 Gs (00), 4 Gs (01), 8 Gs (10), and 16 Gs  (11).
	// Here's a bit of an algorith to calculate DPS/(ADC tick) based on that 2-bit value:
	case AFS_2G:
		aRes = 2.0 / 32768.0;
		break;
	case AFS_4G:
		aRes = 4.0 / 32768.0;
		break;
	case AFS_8G:
		aRes = 8.0 / 32768.0;
		break;
	case AFS_16G:
		aRes = 16.0 / 32768.0;
		break;
	}
}

// Function which accumulates gyro and accelerometer data after device initialization. It calculates the average
// of the at-rest readings and then loads the resulting offsets into accelerometer and gyro bias registers.
void accelgyrocalMPU9250(float * dest1, float * dest2) {
	uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
	uint16_t ii, packet_count, fifo_count;
	int32_t gyro_bias[3] = { 0, 0, 0 }, accel_bias[3] = { 0, 0, 0 };

	// reset device
	writeByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x80); // Write a one to bit 7 reset bit; toggle reset device
	delay(100);

	// get stable time source; Auto select clock source to be PLL gyroscope reference if ready
	// else use the internal oscillator, bits 2:0 = 001
	writeByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x01);
	writeByte(MPU9250_ADDRESS, PWR_MGMT_2, 0x00);
	delay(200);

// Configure device for bias calculation
	writeByte(MPU9250_ADDRESS, INT_ENABLE, 0x00);   // Disable all interrupts
	writeByte(MPU9250_ADDRESS, FIFO_EN, 0x00);      // Disable FIFO
	writeByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x00); // Turn on internal clock source
	writeByte(MPU9250_ADDRESS, I2C_MST_CTRL, 0x00); // Disable I2C master
	writeByte(MPU9250_ADDRESS, USER_CTRL, 0x00); // Disable FIFO and I2C master modes
	writeByte(MPU9250_ADDRESS, USER_CTRL, 0x0C);    // Reset FIFO and DMP
	delay(15);

// Configure MPU6050 gyro and accelerometer for bias calculation
	writeByte(MPU9250_ADDRESS, CONFIG, 0x01);   // Set low-pass filter to 188 Hz
	writeByte(MPU9250_ADDRESS, SMPLRT_DIV, 0x00);  // Set sample rate to 1 kHz
	writeByte(MPU9250_ADDRESS, GYRO_CONFIG, 0x00); // Set gyro full-scale to 250 degrees per second, maximum sensitivity
	writeByte(MPU9250_ADDRESS, ACCEL_CONFIG, 0x00); // Set accelerometer full-scale to 2 g, maximum sensitivity

	uint16_t gyrosensitivity = 131;   // = 131 LSB/degrees/sec
	uint16_t accelsensitivity = 16384;  // = 16384 LSB/g

// Configure FIFO to capture accelerometer and gyro data for bias calculation
	writeByte(MPU9250_ADDRESS, USER_CTRL, 0x40);   // Enable FIFO
	writeByte(MPU9250_ADDRESS, FIFO_EN, 0x78); // Enable gyro and accelerometer sensors for FIFO  (max size 512 bytes in MPU-9150)
	delay(40); // accumulate 40 samples in 40 milliseconds = 480 bytes

// At end of sample accumulation, turn off FIFO sensor read
	writeByte(MPU9250_ADDRESS, FIFO_EN, 0x00); // Disable gyro and accelerometer sensors for FIFO
	readBytes(MPU9250_ADDRESS, FIFO_COUNTH, 2, &data[0]); // read FIFO sample count
	fifo_count = ((uint16_t) data[0] << 8) | data[1];
	packet_count = fifo_count / 12; // How many sets of full gyro and accelerometer data for averaging

	for (ii = 0; ii < packet_count; ii++) {
		int16_t accel_temp[3] = { 0, 0, 0 }, gyro_temp[3] = { 0, 0, 0 };
		readBytes(MPU9250_ADDRESS, FIFO_R_W, 12, &data[0]); // read data for averaging
		accel_temp[0] = (int16_t) (((int16_t) data[0] << 8) | data[1]); // Form signed 16-bit integer for each sample in FIFO
		accel_temp[1] = (int16_t) (((int16_t) data[2] << 8) | data[3]);
		accel_temp[2] = (int16_t) (((int16_t) data[4] << 8) | data[5]);
		gyro_temp[0] = (int16_t) (((int16_t) data[6] << 8) | data[7]);
		gyro_temp[1] = (int16_t) (((int16_t) data[8] << 8) | data[9]);
		gyro_temp[2] = (int16_t) (((int16_t) data[10] << 8) | data[11]);

		accel_bias[0] += (int32_t) accel_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
		accel_bias[1] += (int32_t) accel_temp[1];
		accel_bias[2] += (int32_t) accel_temp[2];
		gyro_bias[0] += (int32_t) gyro_temp[0];
		gyro_bias[1] += (int32_t) gyro_temp[1];
		gyro_bias[2] += (int32_t) gyro_temp[2];

	}
	accel_bias[0] /= (int32_t) packet_count; // Normalize sums to get average count biases
	accel_bias[1] /= (int32_t) packet_count;
	accel_bias[2] /= (int32_t) packet_count;
	gyro_bias[0] /= (int32_t) packet_count;
	gyro_bias[1] /= (int32_t) packet_count;
	gyro_bias[2] /= (int32_t) packet_count;

	if (accel_bias[2] > 0L) {
		accel_bias[2] -= (int32_t) accelsensitivity;
	}  // Remove gravity from the z-axis accelerometer bias calculation
	else {
		accel_bias[2] += (int32_t) accelsensitivity;
	}

// Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
	data[0] = (-gyro_bias[0] / 4 >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
	data[1] = (-gyro_bias[0] / 4) & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
	data[2] = (-gyro_bias[1] / 4 >> 8) & 0xFF;
	data[3] = (-gyro_bias[1] / 4) & 0xFF;
	data[4] = (-gyro_bias[2] / 4 >> 8) & 0xFF;
	data[5] = (-gyro_bias[2] / 4) & 0xFF;

// Push gyro biases to hardware registers
	writeByte(MPU9250_ADDRESS, XG_OFFSET_H, data[0]);
	writeByte(MPU9250_ADDRESS, XG_OFFSET_L, data[1]);
	writeByte(MPU9250_ADDRESS, YG_OFFSET_H, data[2]);
	writeByte(MPU9250_ADDRESS, YG_OFFSET_L, data[3]);
	writeByte(MPU9250_ADDRESS, ZG_OFFSET_H, data[4]);
	writeByte(MPU9250_ADDRESS, ZG_OFFSET_L, data[5]);

// Output scaled gyro biases for display in the main program
	dest1[0] = (float) gyro_bias[0] / (float) gyrosensitivity;
	dest1[1] = (float) gyro_bias[1] / (float) gyrosensitivity;
	dest1[2] = (float) gyro_bias[2] / (float) gyrosensitivity;

// Construct the accelerometer biases for push to the hardware accelerometer bias registers. These registers contain
// factory trim values which must be added to the calculated accelerometer biases; on boot up these registers will hold
// non-zero values. In addition, bit 0 of the lower byte must be preserved since it is used for temperature
// compensation calculations. Accelerometer bias registers expect bias input as 2048 LSB per g, so that
// the accelerometer biases calculated above must be divided by 8.

	int32_t accel_bias_reg[3] = { 0, 0, 0 }; // A place to hold the factory accelerometer trim biases
	readBytes(MPU9250_ADDRESS, XA_OFFSET_H, 2, &data[0]); // Read factory accelerometer trim values
	accel_bias_reg[0] = (int32_t) (((int16_t) data[0] << 8) | data[1]);
	readBytes(MPU9250_ADDRESS, YA_OFFSET_H, 2, &data[0]);
	accel_bias_reg[1] = (int32_t) (((int16_t) data[0] << 8) | data[1]);
	readBytes(MPU9250_ADDRESS, ZA_OFFSET_H, 2, &data[0]);
	accel_bias_reg[2] = (int32_t) (((int16_t) data[0] << 8) | data[1]);

	uint32_t mask = 1uL; // Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers
	uint8_t mask_bit[3] = { 0, 0, 0 }; // Define array to hold mask bit for each accelerometer bias axis

	for (ii = 0; ii < 3; ii++) {
		if ((accel_bias_reg[ii] & mask))
			mask_bit[ii] = 0x01; // If temperature compensation bit is set, record that fact in mask_bit
	}

	// Construct total accelerometer bias, including calculated average accelerometer bias from above
	accel_bias_reg[0] -= (accel_bias[0] / 8); // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale)
	accel_bias_reg[1] -= (accel_bias[1] / 8);
	accel_bias_reg[2] -= (accel_bias[2] / 8);

	data[0] = (accel_bias_reg[0] >> 8) & 0xFE;
	data[1] = (accel_bias_reg[0]) & 0xFE;
	data[1] = data[1] | mask_bit[0]; // preserve temperature compensation bit when writing back to accelerometer bias registers
	data[2] = (accel_bias_reg[1] >> 8) & 0xFE;
	data[3] = (accel_bias_reg[1]) & 0xFE;
	data[3] = data[3] | mask_bit[1]; // preserve temperature compensation bit when writing back to accelerometer bias registers
	data[4] = (accel_bias_reg[2] >> 8) & 0xFE;
	data[5] = (accel_bias_reg[2]) & 0xFE;
	data[5] = data[5] | mask_bit[2]; // preserve temperature compensation bit when writing back to accelerometer bias registers

// Apparently this is not working for the acceleration biases in the MPU-9250
// Are we handling the temperature correction bit properly?
// Push accelerometer biases to hardware registers
	/*  writeByte(MPU9250_ADDRESS, XA_OFFSET_H, data[0]);
	 writeByte(MPU9250_ADDRESS, XA_OFFSET_L, data[1]);
	 writeByte(MPU9250_ADDRESS, YA_OFFSET_H, data[2]);
	 writeByte(MPU9250_ADDRESS, YA_OFFSET_L, data[3]);
	 writeByte(MPU9250_ADDRESS, ZA_OFFSET_H, data[4]);
	 writeByte(MPU9250_ADDRESS, ZA_OFFSET_L, data[5]);
	 */
// Output scaled accelerometer biases for display in the main program
	dest2[0] = (float) accel_bias[0] / (float) accelsensitivity;
	dest2[1] = (float) accel_bias[1] / (float) accelsensitivity;
	dest2[2] = (float) accel_bias[2] / (float) accelsensitivity;
}

void initMPU9250() {
	// wake up device
	writeByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x00); // Clear sleep mode bit (6), enable all sensors
	delay(100); // Wait for all registers to reset

	// get stable time source
	writeByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x01); // Auto select clock source to be PLL gyroscope reference if ready else
	delay(200);

	// Configure Gyro and Thermometer
	// Disable FSYNC and set thermometer and gyro bandwidth to 41 and 42 Hz, respectively;
	// minimum delay time for this setting is 5.9 ms, which means sensor fusion update rates cannot
	// be higher than 1 / 0.0059 = 170 Hz
	// DLPF_CFG = bits 2:0 = 011; this limits the sample rate to 1000 Hz for both
	// With the MPU9250, it is possible to get gyro sample rates of 32 kHz (!), 8 kHz, or 1 kHz
	writeByte(MPU9250_ADDRESS, CONFIG, 0x03);

	// Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
	writeByte(MPU9250_ADDRESS, SMPLRT_DIV, 0x04); // Use a 200 Hz rate; a rate consistent with the filter update rate
	// determined inset in CONFIG above

	// Set gyroscope full scale range
	// Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are left-shifted into positions 4:3
	uint8_t c = readByte(MPU9250_ADDRESS, GYRO_CONFIG); // get current GYRO_CONFIG register value
	// c = c & ~0xE0; // Clear self-test bits [7:5]
	c = c & ~0x02; // Clear Fchoice bits [1:0]
	c = c & ~0x18; // Clear AFS bits [4:3]
	c = c | Gscale << 3; // Set full scale range for the gyro
	// c =| 0x00; // Set Fchoice for the gyro to 11 by writing its inverse to bits 1:0 of GYRO_CONFIG
	writeByte(MPU9250_ADDRESS, GYRO_CONFIG, c); // Write new GYRO_CONFIG value to register

	// Set accelerometer full-scale range configuration
	c = readByte(MPU9250_ADDRESS, ACCEL_CONFIG); // get current ACCEL_CONFIG register value
	// c = c & ~0xE0; // Clear self-test bits [7:5]
	c = c & ~0x18;  // Clear AFS bits [4:3]
	c = c | Ascale << 3; // Set full scale range for the accelerometer
	writeByte(MPU9250_ADDRESS, ACCEL_CONFIG, c); // Write new ACCEL_CONFIG register value

	// Set accelerometer sample rate configuration
	// It is possible to get a 4 kHz sample rate from the accelerometer by choosing 1 for
	// accel_fchoice_b bit [3]; in this case the bandwidth is 1.13 kHz
	c = readByte(MPU9250_ADDRESS, ACCEL_CONFIG2); // get current ACCEL_CONFIG2 register value
	c = c & ~0x0F; // Clear accel_fchoice_b (bit 3) and A_DLPFG (bits [2:0])
	c = c | 0x03;  // Set accelerometer rate to 1 kHz and bandwidth to 41 Hz
	writeByte(MPU9250_ADDRESS, ACCEL_CONFIG2, c); // Write new ACCEL_CONFIG2 register value

	// The accelerometer, gyro, and thermometer are set to 1 kHz sample rates,
	// but all these rates are further reduced by a factor of 5 to 200 Hz because of the SMPLRT_DIV setting

	// Configure Interrupts and Bypass Enable
	// Set interrupt pin active high, push-pull, hold interrupt pin level HIGH until interrupt cleared,
	// clear on read of INT_STATUS, and enable I2C_BYPASS_EN so additional chips
	// can join the I2C bus and all can be controlled by the Arduino as master
	writeByte(MPU9250_ADDRESS, INT_PIN_CFG, 0x22);
	writeByte(MPU9250_ADDRESS, INT_ENABLE, 0x01); // Enable data ready (bit 0) interrupt
	delay(100);
}

//AK8963C magnetometer
void initAK8963(float * destination) {
	// First extract the factory calibration for each magnetometer axis
	uint8_t rawData[3];  // x/y/z gyro calibration data stored here
	writeByte(AK8963_ADDRESS, AK8963_CNTL, 0x00); // Power down magnetometer
	delay(20);
	writeByte(AK8963_ADDRESS, AK8963_CNTL, 0x0F); // Enter Fuse ROM access mode
	delay(20);
	readBytes(AK8963_ADDRESS, AK8963_ASAX, 3, &rawData[0]); // Read the x-, y-, and z-axis calibration values
	destination[0] = (float) (rawData[0] - 128) / 256. + 1.; // Return x-axis sensitivity adjustment values, etc.
	destination[1] = (float) (rawData[1] - 128) / 256. + 1.;
	destination[2] = (float) (rawData[2] - 128) / 256. + 1.;
	writeByte(AK8963_ADDRESS, AK8963_CNTL, 0x00); // Power down magnetometer
	delay(20);
	// Configure the magnetometer for continuous read and highest resolution
	// set Mscale bit 4 to 1 (0) to enable 16 (14) bit resolution in CNTL register,
	// and enable continuous mode data acquisition Mmode (bits [3:0]), 0010 for 8 Hz and 0110 for 100 Hz sample rates
	writeByte(AK8963_ADDRESS, AK8963_CNTL, Mscale << 4 | Mmode); // Set magnetometer data resolution and sample ODR
	delay(20);
}

void readMagData(int16_t * destination) {
	uint8_t rawData[7]; // x/y/z gyro register data, ST2 register stored here, must read ST2 at end of data acquisition
	if (readByte(AK8963_ADDRESS, AK8963_ST1) & 0x01) { // wait for magnetometer data ready bit to be set
		readBytes(AK8963_ADDRESS, AK8963_XOUT_L, 7, &rawData[0]); // Read the six raw data and ST2 registers sequentially into data array
		uint8_t c = rawData[6]; // End data read by reading ST2 register
		if (!(c & 0x08)) { // Check if magnetic sensor overflow set, if not then report data
			destination[0] = ((int16_t) rawData[1] << 8) | rawData[0]; // Turn the MSB and LSB into a signed 16-bit value
			destination[1] = ((int16_t) rawData[3] << 8) | rawData[2]; // Data stored as little Endian
			destination[2] = ((int16_t) rawData[5] << 8) | rawData[4];
		}
	}
}

void magcalMPU9250(float * dest1, float * dest2) {
	uint16_t ii = 0, sample_count = 0;
	int32_t mag_bias[3] = { 0, 0, 0 }, mag_scale[3] = { 0, 0, 0 };
	int16_t mag_max[3] = { 0xFF, 0xFF, 0xFF },
			mag_min[3] = { 0x7F, 0x7F, 0x7F }, mag_temp[3] = { 0, 0, 0 };

	Serial.println(
			"Mag Calibration: Wave device in a figure eight until done!");
	delay(4000);

	if (Mmode == 0x02)
		sample_count = 128;
	if (Mmode == 0x06)
		sample_count = 1500;
	for (ii = 0; ii < sample_count; ii++) {
		readMagData(mag_temp);  // Read the mag data
		for (int jj = 0; jj < 3; jj++) {
			if (mag_temp[jj] > mag_max[jj])
				mag_max[jj] = mag_temp[jj];
			if (mag_temp[jj] < mag_min[jj])
				mag_min[jj] = mag_temp[jj];
		}
		if (Mmode == 0x02)
			delay(135);  // at 8 Hz ODR, new mag data is available every 125 ms
		if (Mmode == 0x06)
			delay(12);  // at 100 Hz ODR, new mag data is available every 10 ms
	}

//    Serial.println("mag x min/max:"); Serial.println(mag_max[0]); Serial.println(mag_min[0]);
//    Serial.println("mag y min/max:"); Serial.println(mag_max[1]); Serial.println(mag_min[1]);
//    Serial.println("mag z min/max:"); Serial.println(mag_max[2]); Serial.println(mag_min[2]);

	// Get hard iron correction
	mag_bias[0] = (mag_max[0] + mag_min[0]) / 2; // get average x mag bias in counts
	mag_bias[1] = (mag_max[1] + mag_min[1]) / 2; // get average y mag bias in counts
	mag_bias[2] = (mag_max[2] + mag_min[2]) / 2; // get average z mag bias in counts

	dest1[0] = (float) mag_bias[0] * mRes * magCalibration[0]; // save mag biases in G for main program
	dest1[1] = (float) mag_bias[1] * mRes * magCalibration[1];
	dest1[2] = (float) mag_bias[2] * mRes * magCalibration[2];

	// Get soft iron correction estimate
	mag_scale[0] = (mag_max[0] - mag_min[0]) / 2; // get average x axis max chord length in counts
	mag_scale[1] = (mag_max[1] - mag_min[1]) / 2; // get average y axis max chord length in counts
	mag_scale[2] = (mag_max[2] - mag_min[2]) / 2; // get average z axis max chord length in counts

	float avg_rad = mag_scale[0] + mag_scale[1] + mag_scale[2];
	avg_rad /= 3.0;

	dest2[0] = avg_rad / ((float) mag_scale[0]);
	dest2[1] = avg_rad / ((float) mag_scale[1]);
	dest2[2] = avg_rad / ((float) mag_scale[2]);

	Serial.println("Mag Calibration done!");
}

void setup() {
	// Setup for Master mode, pins 18/19, external pullups, 400kHz for Teensy 3.1
	Wire.begin(I2C_MASTER, 0x00, I2C_PINS_16_17, I2C_PULLUP_EXT, I2C_RATE_400);
	delay(5000);
	Serial.begin(38400);

	// Set up the interrupt pin, its set as active high, push-pull
	pinMode(LED, OUTPUT);
	digitalWrite(LED, LOW);

	// Detect Sentral at 0x28
	I2Cscan();

	// Read SENtral device information
	uint16_t ROM1 = readByte(EM7180_ADDRESS, EM7180_ROMVersion1);
	uint16_t ROM2 = readByte(EM7180_ADDRESS, EM7180_ROMVersion2);
	Serial.print("EM7180 ROM Version: 0x");
	Serial.print(ROM1, HEX);
	Serial.println(ROM2, HEX);
	Serial.println("Should be: 0xE69");
	uint16_t RAM1 = readByte(EM7180_ADDRESS, EM7180_RAMVersion1);
	uint16_t RAM2 = readByte(EM7180_ADDRESS, EM7180_RAMVersion2);
	Serial.print("EM7180 RAM Version: 0x");
	Serial.print(RAM1);
	Serial.println(RAM2);
	uint8_t PID = readByte(EM7180_ADDRESS, EM7180_ProductID);
	Serial.print("EM7180 ProductID: 0x");
	Serial.print(PID, HEX);
	Serial.println(" Should be: 0x80");
	uint8_t RID = readByte(EM7180_ADDRESS, EM7180_RevisionID);
	Serial.print("EM7180 RevisionID: 0x");
	Serial.print(RID, HEX);
	Serial.println(" Should be: 0x02");

	delay(1000); // give some time to read the screen

	// Check which sensors can be detected by the EM7180
	uint8_t featureflag = readByte(EM7180_ADDRESS, EM7180_FeatureFlags);
	if (featureflag & 0x01)
		Serial.println("A barometer is installed");
	if (featureflag & 0x02)
		Serial.println("A humidity sensor is installed");
	if (featureflag & 0x04)
		Serial.println("A temperature sensor is installed");
	if (featureflag & 0x08)
		Serial.println("A custom sensor is installed");
	if (featureflag & 0x10)
		Serial.println("A second custom sensor is installed");
	if (featureflag & 0x20)
		Serial.println("A third custom sensor is installed");

	delay(1000); // give some time to read the screen

	byte STAT = (readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x01);
	if (readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x01)
		Serial.println("EEPROM detected on the sensor bus!");
	if (readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x02)
		Serial.println("EEPROM uploaded config file!");
	if (readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x04)
		Serial.println("EEPROM CRC incorrect!");
	if (readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x08)
		Serial.println("EM7180 in initialized state!");
	if (readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x10)
		Serial.println("No EEPROM detected!");

	int count = 0;
	while (!STAT) {
		writeByte(EM7180_ADDRESS, EM7180_ResetRequest, 0x01);
		delay(500);
		count++;
		STAT = (readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x01);
		if (readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x01)
			Serial.println("EEPROM detected on the sensor bus!");
		if (readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x02)
			Serial.println("EEPROM uploaded config file!");
		if (readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x04)
			Serial.println("EEPROM CRC incorrect!");
		if (readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x08)
			Serial.println("EM7180 in initialized state!");
		if (readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x10)
			Serial.println("No EEPROM detected!");
		if (count > 10)
			break;
	}

	if (!(readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x04))
		Serial.println("EEPROM upload successful!");
	delay(1000); // give some time to read the screen

	// Set up the SENtral as sensor bus in normal operating mode
	if (!passThru) {
		// Enter EM7180 initialized state
		writeByte(EM7180_ADDRESS, EM7180_HostControl, 0x00); // set SENtral in initialized state to configure registers
		writeByte(EM7180_ADDRESS, EM7180_PassThruControl, 0x00); // make sure pass through mode is off
		writeByte(EM7180_ADDRESS, EM7180_HostControl, 0x01); // Force initialize
		writeByte(EM7180_ADDRESS, EM7180_HostControl, 0x00); // set SENtral in initialized state to configure registers

		Serial.println("EM7180 initialized state");

		//Setup LPF bandwidth (BEFORE setting ODR's)
		writeByte(EM7180_ADDRESS, EM7180_ACC_LPF_BW, 0x03); // 41Hz
		writeByte(EM7180_ADDRESS, EM7180_GYRO_LPF_BW, 0x03); // 41Hz
		// Set accel/gyro/mage desired ODR rates
		writeByte(EM7180_ADDRESS, EM7180_QRateDivisor, 0x02); // 100 Hz
		writeByte(EM7180_ADDRESS, EM7180_MagRate, 0x64); // 100 Hz
		writeByte(EM7180_ADDRESS, EM7180_AccelRate, 0x14); // 200/10 Hz
		writeByte(EM7180_ADDRESS, EM7180_GyroRate, 0x14); // 200/10 Hz
		writeByte(EM7180_ADDRESS, EM7180_BaroRate, 0x80 | 0x32); // set enable bit and set Baro rate to 25 Hz
		// writeByte(EM7180_ADDRESS, EM7180_TempRate, 0x19);  // set enable bit and set rate to 25 Hz
		Serial.println("Setup LPF bandwidth");

		// Configure operating mode
		writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x00); // read scale sensor data
		// Enable interrupt to host upon certain events
		// choose host interrupts when any sensor updated (0x40), new gyro data (0x20), new accel data (0x10),
		// new mag data (0x08), quaternions updated (0x04), an error occurs (0x02), or the SENtral needs to be reset(0x01)
		writeByte(EM7180_ADDRESS, EM7180_EnableEvents, 0x07);
		// Enable EM7180 run mode
		writeByte(EM7180_ADDRESS, EM7180_HostControl, 0x01); // set SENtral in normal run mode
		delay(100);
		Serial.println("Configure operating mode");

		// EM7180 parameter adjustments
		Serial.println("Beginning Parameter Adjustments");

		// Read sensor default FS values from parameter space
		writeByte(EM7180_ADDRESS, EM7180_ParamRequest, 0x4A); // Request to read parameter 74
		writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x80); // Request parameter transfer process
		byte param_xfer = readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge);
		while (!(param_xfer == 0x4A)) {
			param_xfer = readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge);
		}
		param[0] = readByte(EM7180_ADDRESS, EM7180_SavedParamByte0);
		param[1] = readByte(EM7180_ADDRESS, EM7180_SavedParamByte1);
		param[2] = readByte(EM7180_ADDRESS, EM7180_SavedParamByte2);
		param[3] = readByte(EM7180_ADDRESS, EM7180_SavedParamByte3);
		EM7180_mag_fs = ((int16_t) (param[1] << 8) | param[0]);
		EM7180_acc_fs = ((int16_t) (param[3] << 8) | param[2]);
		Serial.print("Magnetometer Default Full Scale Range: +/-");
		Serial.print(EM7180_mag_fs);
		Serial.println("uT");
		Serial.print("Accelerometer Default Full Scale Range: +/-");
		Serial.print(EM7180_acc_fs);
		Serial.println("g");
		writeByte(EM7180_ADDRESS, EM7180_ParamRequest, 0x4B); // Request to read  parameter 75
		param_xfer = readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge);
		while (!(param_xfer == 0x4B)) {
			param_xfer = readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge);
		}
		param[0] = readByte(EM7180_ADDRESS, EM7180_SavedParamByte0);
		param[1] = readByte(EM7180_ADDRESS, EM7180_SavedParamByte1);
		param[2] = readByte(EM7180_ADDRESS, EM7180_SavedParamByte2);
		param[3] = readByte(EM7180_ADDRESS, EM7180_SavedParamByte3);
		EM7180_gyro_fs = ((int16_t) (param[1] << 8) | param[0]);
		Serial.print("Gyroscope Default Full Scale Range: +/-");
		Serial.print(EM7180_gyro_fs);
		Serial.println("dps");
		writeByte(EM7180_ADDRESS, EM7180_ParamRequest, 0x00); //End parameter transfer
		writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x00); // re-enable algorithm

		//Disable stillness mode
		EM7180_set_integer_param(0x49, 0x00);

		//Write desired sensor full scale ranges to the EM7180
		EM7180_set_mag_acc_FS(0x3E8, 0x08); // 1000 uT, 8 g
		EM7180_set_gyro_FS(0x7D0); // 2000 dps

		// Read sensor new FS values from parameter space
		writeByte(EM7180_ADDRESS, EM7180_ParamRequest, 0x4A); // Request to read  parameter 74
		writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x80); // Request parameter transfer process
		param_xfer = readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge);
		while (!(param_xfer == 0x4A)) {
			param_xfer = readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge);
		}
		param[0] = readByte(EM7180_ADDRESS, EM7180_SavedParamByte0);
		param[1] = readByte(EM7180_ADDRESS, EM7180_SavedParamByte1);
		param[2] = readByte(EM7180_ADDRESS, EM7180_SavedParamByte2);
		param[3] = readByte(EM7180_ADDRESS, EM7180_SavedParamByte3);
		EM7180_mag_fs = ((int16_t) (param[1] << 8) | param[0]);
		EM7180_acc_fs = ((int16_t) (param[3] << 8) | param[2]);
		Serial.print("Magnetometer New Full Scale Range: +/-");
		Serial.print(EM7180_mag_fs);
		Serial.println("uT");
		Serial.print("Accelerometer New Full Scale Range: +/-");
		Serial.print(EM7180_acc_fs);
		Serial.println("g");
		writeByte(EM7180_ADDRESS, EM7180_ParamRequest, 0x4B); // Request to read  parameter 75
		param_xfer = readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge);
		while (!(param_xfer == 0x4B)) {
			param_xfer = readByte(EM7180_ADDRESS, EM7180_ParamAcknowledge);
		}
		param[0] = readByte(EM7180_ADDRESS, EM7180_SavedParamByte0);
		param[1] = readByte(EM7180_ADDRESS, EM7180_SavedParamByte1);
		param[2] = readByte(EM7180_ADDRESS, EM7180_SavedParamByte2);
		param[3] = readByte(EM7180_ADDRESS, EM7180_SavedParamByte3);
		EM7180_gyro_fs = ((int16_t) (param[1] << 8) | param[0]);
		Serial.print("Gyroscope New Full Scale Range: +/-");
		Serial.print(EM7180_gyro_fs);
		Serial.println("dps");
		writeByte(EM7180_ADDRESS, EM7180_ParamRequest, 0x00); //End parameter transfer
		writeByte(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x00); // re-enable algorithm

		// Read EM7180 status
		uint8_t runStatus = readByte(EM7180_ADDRESS, EM7180_RunStatus);
		if (runStatus & 0x01)
			Serial.println(" EM7180 run status = normal mode");
		uint8_t algoStatus = readByte(EM7180_ADDRESS, EM7180_AlgorithmStatus);
		if (algoStatus & 0x01)
			Serial.println(" EM7180 standby status");
		if (algoStatus & 0x02)
			Serial.println(" EM7180 algorithm slow");
		if (algoStatus & 0x04)
			Serial.println(" EM7180 in stillness mode");
		if (algoStatus & 0x08)
			Serial.println(" EM7180 mag calibration completed");
		if (algoStatus & 0x10)
			Serial.println(" EM7180 magnetic anomaly detected");
		if (algoStatus & 0x20)
			Serial.println(" EM7180 unreliable sensor data");
		uint8_t passthruStatus = readByte(EM7180_ADDRESS,
		EM7180_PassThruStatus);
		if (passthruStatus & 0x01)
			Serial.print(" EM7180 in passthru mode!");
		uint8_t eventStatus = readByte(EM7180_ADDRESS, EM7180_EventStatus);
		if (eventStatus & 0x01)
			Serial.println(" EM7180 CPU reset");
		if (eventStatus & 0x02)
			Serial.println(" EM7180 Error");
		if (eventStatus & 0x04)
			Serial.println(" EM7180 new quaternion result");
		if (eventStatus & 0x08)
			Serial.println(" EM7180 new mag result");
		if (eventStatus & 0x10)
			Serial.println(" EM7180 new accel result");
		if (eventStatus & 0x20)
			Serial.println(" EM7180 new gyro result");

		delay(1000); // give some time to read the screen

		// Check sensor status
		uint8_t sensorStatus = readByte(EM7180_ADDRESS, EM7180_SensorStatus);
		Serial.print(" EM7180 sensor status = ");
		Serial.println(sensorStatus);
		if (sensorStatus & 0x01)
			Serial.print("Magnetometer not acknowledging!");
		if (sensorStatus & 0x02)
			Serial.print("Accelerometer not acknowledging!");
		if (sensorStatus & 0x04)
			Serial.print("Gyro not acknowledging!");
		if (sensorStatus & 0x10)
			Serial.print("Magnetometer ID not recognized!");
		if (sensorStatus & 0x20)
			Serial.print("Accelerometer ID not recognized!");
		if (sensorStatus & 0x40)
			Serial.print("Gyro ID not recognized!");

		Serial.print("Actual MagRate = ");
		Serial.print(readByte(EM7180_ADDRESS, EM7180_ActualMagRate));
		Serial.println(" Hz");
		Serial.print("Actual AccelRate = ");
		Serial.print(10 * readByte(EM7180_ADDRESS, EM7180_ActualAccelRate));
		Serial.println(" Hz");
		Serial.print("Actual GyroRate = ");
		Serial.print(10 * readByte(EM7180_ADDRESS, EM7180_ActualGyroRate));
		Serial.println(" Hz");
		Serial.print("Actual BaroRate = ");
		Serial.print(readByte(EM7180_ADDRESS, EM7180_ActualBaroRate));
		Serial.println(" Hz");
		//  Serial.print("Actual TempRate = "); Serial.print(readByte(EM7180_ADDRESS, EM7180_ActualTempRate)); Serial.println(" Hz");

		delay(1000); // give some time to read the screen

	}

	// If pass through mode desired, set it up here
	if (passThru) {
		// Put EM7180 SENtral into pass-through mode
		SENtralPassThroughMode();
		delay(1000);

		I2Cscan(); // should see all the devices on the I2C bus including two from the EEPROM (ID page and data pages)

		// Read first page of EEPROM
		uint8_t data[128];
		M24512DFMreadBytes(M24512DFM_DATA_ADDRESS, 0x00, 0x00, 128, data);
		Serial.println("EEPROM Signature Byte");
		Serial.print(data[0], HEX);
		Serial.println("  Should be 0x2A");
		Serial.print(data[1], HEX);
		Serial.println("  Should be 0x65");
		for (int i = 0; i < 128; i++) {
			Serial.print(data[i], HEX);
			Serial.print(" ");
		}

		// Set up the interrupt pin, its set as active high, push-pull
		pinMode(LED, OUTPUT);
		digitalWrite(LED, HIGH);

		// Read the WHO_AM_I register, this is a good test of communication
		Serial.println("MPU9250 9-axis motion sensor...");
		byte c = readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250); // Read WHO_AM_I register for MPU-9250
		Serial.print("MPU9250 ");
		Serial.print("I AM ");
		Serial.print(c, HEX);
		Serial.print(" I should be ");
		Serial.println(0x71, HEX);
		if (c == 0x71) // WHO_AM_I should always be 0x71
				{
			Serial.println("MPU9250 is online...");
			MPU9250SelfTest(SelfTest); // Start by performing self test and reporting values
			Serial.print("x-axis self test: acceleration trim within : ");
//			Serial.print(SelfTest[0], 1);
			Serial.println("% of factory value");
			Serial.print("y-axis self test: acceleration trim within : ");
//			Serial.print(SelfTest[1], 1);
			Serial.println("% of factory value");
			Serial.print("z-axis self test: acceleration trim within : ");
//			Serial.print(SelfTest[2], 1);
			Serial.println("% of factory value");
			Serial.print("x-axis self test: gyration trim within : ");
//			Serial.print(SelfTest[3], 1);
			Serial.println("% of factory value");
			Serial.print("y-axis self test: gyration trim within : ");
//			Serial.print(SelfTest[4], 1);
			Serial.println("% of factory value");
			Serial.print("z-axis self test: gyration trim within : ");
//			Serial.print(SelfTest[5], 1);
			Serial.println("% of factory value");
			delay(1000);

			// get sensor resolutions, only need to do this once
			getAres();
			getGres();
			getMres();

			Serial.println(" Calibrate gyro and accel");
			accelgyrocalMPU9250(gyroBias, accelBias); // Calibrate gyro and accelerometers, load biases in bias registers
			Serial.println("accel biases (mg)");
			Serial.println(1000. * accelBias[0]);
			Serial.println(1000. * accelBias[1]);
			Serial.println(1000. * accelBias[2]);
			Serial.println("gyro biases (dps)");
			Serial.println(gyroBias[0]);
			Serial.println(gyroBias[1]);
			Serial.println(gyroBias[2]);

			delay(1000);

			initMPU9250();
			Serial.println("MPU9250 initialized for active data mode...."); // Initialize device for active mode read of acclerometer, gyroscope, and temperature
			writeByte(MPU9250_ADDRESS, INT_PIN_CFG, 0x22);
			I2Cscan(); // should see all the devices on the I2C bus including two from the EEPROM (ID page and data pages)

			// Read the WHO_AM_I register of the magnetometer, this is a good test of communication
			byte d = readByte(AK8963_ADDRESS, WHO_AM_I_AK8963); // Read WHO_AM_I register for AK8963
			Serial.print("AK8963 ");
			Serial.print("I AM ");
			Serial.print(d, HEX);
			Serial.print(" I should be ");
			Serial.println(0x48, HEX);

			delay(1000);

			// Get magnetometer calibration from AK8963 ROM
			initAK8963(magCalibration);
			Serial.println("AK8963 initialized for active data mode...."); // Initialize device for active mode read of magnetometer

			magcalMPU9250(magBias, magScale);
			Serial.println("AK8963 mag biases (mG)");
			Serial.println(magBias[0]);
			Serial.println(magBias[1]);
			Serial.println(magBias[2]);
			Serial.println("AK8963 mag scale (mG)");
			Serial.println(magScale[0]);
			Serial.println(magScale[1]);
			Serial.println(magScale[2]);
			delay(2000); // add delay to see results before serial spew of data

			if (SerialDebug) {
				//  Serial.println("Calibration values: ");
				Serial.print("X-Axis sensitivity adjustment value ");
				Serial.println(magCalibration[0], 2);
				Serial.print("Y-Axis sensitivity adjustment value ");
				Serial.println(magCalibration[1], 2);
				Serial.print("Z-Axis sensitivity adjustment value ");
				Serial.println(magCalibration[2], 2);
			}

			delay(1000);

			// Read the WHO_AM_I register of the BMP280 this is a good test of communication
			byte f = readByte(BMP280_ADDRESS, BMP280_ID); // Read WHO_AM_I register for BMP280
			Serial.print("BMP280 ");
			Serial.print("I AM ");
			Serial.print(f, HEX);
			Serial.print(" I should be ");
			Serial.println(0x58, HEX);
			Serial.println(" ");

			delay(1000);

			writeByte(BMP280_ADDRESS, BMP280_RESET, 0xB6); // reset BMP280 before initilization
			delay(100);

			BMP280Init(); // Initialize BMP280 altimeter
			Serial.println("Calibration coeficients:");
			Serial.print("dig_T1 =");
			Serial.println(dig_T1);
			Serial.print("dig_T2 =");
			Serial.println(dig_T2);
			Serial.print("dig_T3 =");
			Serial.println(dig_T3);
			Serial.print("dig_P1 =");
			Serial.println(dig_P1);
			Serial.print("dig_P2 =");
			Serial.println(dig_P2);
			Serial.print("dig_P3 =");
			Serial.println(dig_P3);
			Serial.print("dig_P4 =");
			Serial.println(dig_P4);
			Serial.print("dig_P5 =");
			Serial.println(dig_P5);
			Serial.print("dig_P6 =");
			Serial.println(dig_P6);
			Serial.print("dig_P7 =");
			Serial.println(dig_P7);
			Serial.print("dig_P8 =");
			Serial.println(dig_P8);
			Serial.print("dig_P9 =");
			Serial.println(dig_P9);

			delay(1000);

		} else {
			Serial.print("Could not connect to MPU9250: 0x");
			Serial.println(c, HEX);
			while (1)
				; // Loop forever if communication doesn't happen
		}
	}

}

float ax, ay, az, gx, gy, gz, mx, my, mz; // variables to hold latest sensor data values
int16_t tempCount, rawPressure, rawTemperature; // pressure, temperature raw count output
float temperature, pressure, altitude; // Stores the MPU9250 internal chip temperature in degrees Celsius

int16_t accelCount[3];  // Stores the 16-bit signed accelerometer sensor output
int16_t gyroCount[3];   // Stores the 16-bit signed gyro sensor output
int16_t magCount[3]; // Stores the 16-bit signed magnetometer sensor output

int32_t rawPress, rawTemp; // pressure and temperature raw count output for BMP280
float pitch, yaw, roll, Yaw, Pitch, Roll;
float q[4] = { 1.0f, 0.0f, 0.0f, 0.0f }; // vector to hold quaternion
float Quat[4] = { 0, 0, 0, 0 }; // quaternion data register

uint32_t delt_t = 0, count = 0, sumCount = 0; // used to control display output rate
float deltat = 0.0f, sum = 0.0f; // integration interval for both filter schemes

uint32_t lastUpdate = 0, firstUpdate = 0; // used to calculate integration interval
uint32_t Now = 0; // used to calculate integration interval

void readSENtralAccelData(int16_t * destination) {
	uint8_t rawData[6];  // x/y/z accel register data stored here
	readBytes(EM7180_ADDRESS, EM7180_AX, 6, &rawData[0]); // Read the six raw data registers into data array
	destination[0] = (int16_t) (((int16_t) rawData[1] << 8) | rawData[0]); // Turn the MSB and LSB into a signed 16-bit value
	destination[1] = (int16_t) (((int16_t) rawData[3] << 8) | rawData[2]);
	destination[2] = (int16_t) (((int16_t) rawData[5] << 8) | rawData[4]);
}
// The loop function is called in an endless loop

void readSENtralGyroData(int16_t * destination) {
	uint8_t rawData[6];  // x/y/z gyro register data stored here
	readBytes(EM7180_ADDRESS, EM7180_GX, 6, &rawData[0]); // Read the six raw data registers sequentially into data array
	destination[0] = (int16_t) (((int16_t) rawData[1] << 8) | rawData[0]); // Turn the MSB and LSB into a signed 16-bit value
	destination[1] = (int16_t) (((int16_t) rawData[3] << 8) | rawData[2]);
	destination[2] = (int16_t) (((int16_t) rawData[5] << 8) | rawData[4]);
}

void readSENtralMagData(int16_t * destination) {
	uint8_t rawData[6];  // x/y/z gyro register data stored here
	readBytes(EM7180_ADDRESS, EM7180_MX, 6, &rawData[0]); // Read the six raw data registers sequentially into data array
	destination[0] = (int16_t) (((int16_t) rawData[1] << 8) | rawData[0]); // Turn the MSB and LSB into a signed 16-bit value
	destination[1] = (int16_t) (((int16_t) rawData[3] << 8) | rawData[2]);
	destination[2] = (int16_t) (((int16_t) rawData[5] << 8) | rawData[4]);
}

int16_t readSENtralBaroData() {
	uint8_t rawData[2];  // x/y/z gyro register data stored here
	readBytes(EM7180_ADDRESS, EM7180_Baro, 2, &rawData[0]); // Read the two raw data registers sequentially into data array
	return (int16_t) (((int16_t) rawData[1] << 8) | rawData[0]); // Turn the MSB and LSB into a signed 16-bit value
}

int16_t readSENtralTempData() {
	uint8_t rawData[2];  // x/y/z gyro register data stored here
	readBytes(EM7180_ADDRESS, EM7180_Temp, 2, &rawData[0]); // Read the two raw data registers sequentially into data array
	return (int16_t) (((int16_t) rawData[1] << 8) | rawData[0]); // Turn the MSB and LSB into a signed 16-bit value
}

void readAccelData(int16_t * destination) {
	uint8_t rawData[6];  // x/y/z accel register data stored here
	readBytes(MPU9250_ADDRESS, ACCEL_XOUT_H, 6, &rawData[0]); // Read the six raw data registers into data array
	destination[0] = ((int16_t) rawData[0] << 8) | rawData[1]; // Turn the MSB and LSB into a signed 16-bit value
	destination[1] = ((int16_t) rawData[2] << 8) | rawData[3];
	destination[2] = ((int16_t) rawData[4] << 8) | rawData[5];
}

void readGyroData(int16_t * destination) {
	uint8_t rawData[6];  // x/y/z gyro register data stored here
	readBytes(MPU9250_ADDRESS, GYRO_XOUT_H, 6, &rawData[0]); // Read the six raw data registers sequentially into data array
	destination[0] = ((int16_t) rawData[0] << 8) | rawData[1]; // Turn the MSB and LSB into a signed 16-bit value
	destination[1] = ((int16_t) rawData[2] << 8) | rawData[3];
	destination[2] = ((int16_t) rawData[4] << 8) | rawData[5];
}

void loop() {
	if (!passThru) {
		// Check event status register, way to chech data ready by polling rather than interrupt
		uint8_t eventStatus = readByte(EM7180_ADDRESS, EM7180_EventStatus); // reading clears the register

		// Check for errors
		if (eventStatus & 0x02) { // error detected, what is it?

			uint8_t errorStatus = readByte(EM7180_ADDRESS,
			EM7180_ErrorRegister);
			if (errorStatus != 0x00) { // non-zero value indicates error, what is it?
				Serial.print(" EM7180 sensor status = ");
				Serial.println(errorStatus);
				if (errorStatus == 0x11)
					Serial.print("Magnetometer failure!");
				if (errorStatus == 0x12)
					Serial.print("Accelerometer failure!");
				if (errorStatus == 0x14)
					Serial.print("Gyro failure!");
				if (errorStatus == 0x21)
					Serial.print("Magnetometer initialization failure!");
				if (errorStatus == 0x22)
					Serial.print("Accelerometer initialization failure!");
				if (errorStatus == 0x24)
					Serial.print("Gyro initialization failure!");
				if (errorStatus == 0x30)
					Serial.print("Math error!");
				if (errorStatus == 0x80)
					Serial.print("Invalid sample rate!");
			}

			// Handle errors ToDo

		}

		// if no errors, see if new data is ready
		if (eventStatus & 0x10) { // new acceleration data available
			readSENtralAccelData(accelCount);

			// Now we'll calculate the accleration value into actual g's
			ax = (float) accelCount[0] * 0.000488;  // get actual g value
			ay = (float) accelCount[1] * 0.000488;
			az = (float) accelCount[2] * 0.000488;
		}

		if (eventStatus & 0x20) { // new gyro data available
			readSENtralGyroData(gyroCount);

			// Now we'll calculate the gyro value into actual dps's
			gx = (float) gyroCount[0] * 0.153;  // get actual dps value
			gy = (float) gyroCount[1] * 0.153;
			gz = (float) gyroCount[2] * 0.153;
		}

		if (eventStatus & 0x08) { // new mag data available
			readSENtralMagData(magCount);

			// Now we'll calculate the mag value into actual G's
			mx = (float) magCount[0] * 0.305176;  // get actual G value
			my = (float) magCount[1] * 0.305176;
			mz = (float) magCount[2] * 0.305176;
		}

//		if (eventStatus & 0x04) { // new quaternion data available
//			readSENtralQuatData(Quat);
//		}

		// get BMP280 pressure
		if (eventStatus & 0x40) { // new baro data available
			//   Serial.println("new Baro data!");
			rawPressure = readSENtralBaroData();
			pressure = (float) rawPressure * 0.01f + 1013.25f; // pressure in mBar

			// get BMP280 temperature
			rawTemperature = readSENtralTempData();
			temperature = (float) rawTemperature * 0.01; // temperature in degrees C
		}
	}

	if (passThru) {
		// If intPin goes high, all data registers have new data
		readAccelData(accelCount);  // Read the x/y/z adc values

		// Now we'll calculate the acceleration value into actual g's
		ax = (float) accelCount[0] * aRes - accelBias[0]; // get actual g value, this depends on scale being set
		ay = (float) accelCount[1] * aRes - accelBias[1];
		az = (float) accelCount[2] * aRes - accelBias[2];
		// }
//  if (digitalRead(intGYRO2)) {  // On interrupt, read data
		readGyroData(gyroCount);  // Read the x/y/z adc values

		// Calculate the gyro value into actual degrees per second
		gx = (float) gyroCount[0] * gRes; // get actual gyro value, this depends on scale being set
		gy = (float) gyroCount[1] * gRes;
		gz = (float) gyroCount[2] * gRes;
		// }
//  if (digitalRead(intDRDYM)) {  // On interrupt, read data
		readMagData(magCount);  // Read the x/y/z adc values

		// Calculate the magnetometer values in milliGauss
		mx = (float) magCount[0] * mRes * magCalibration[0] - magBias[0]; // get actual magnetometer value, this depends on scale being set
		my = (float) magCount[1] * mRes * magCalibration[1] - magBias[1];
		mz = (float) magCount[2] * mRes * magCalibration[2] - magBias[2];
//    mx *= magScale[0];
//    my *= magScale[1];
//    mz *= magScale[2];
		//   }
	}

	// keep track of rates
	Now = micros();
	deltat = ((Now - lastUpdate) / 1000000.0f); // set integration time by time elapsed since last filter update
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
//	MadgwickQuaternionUpdate(-ay, -ax, az, gy * PI / 180.0f, gx * PI / 180.0f,
//			-gz * PI / 180.0f, mx, my, mz);
//  if(passThru)MahonyQuaternionUpdate(-ay, -ax, az, gy*PI/180.0f, gx*PI/180.0f, -gz*PI/180.0f,  mx,  my, mz);

	// Serial print and/or display at 0.5 s rate independent of data rates
	delt_t = millis() - count;
	if (delt_t > 500) { // update LCD once per half-second independent of read rate

		if (SerialDebug) {
			Serial.print("ax = ");
			Serial.print((int) 1000 * ax);
			Serial.print(" ay = ");
			Serial.print((int) 1000 * ay);
			Serial.print(" az = ");
			Serial.print((int) 1000 * az);
			Serial.println(" mg");
			Serial.print("gx = ");
			Serial.print(gx, 2);
			Serial.print(" gy = ");
			Serial.print(gy, 2);
			Serial.print(" gz = ");
			Serial.print(gz, 2);
			Serial.println(" deg/s");
			Serial.print("mx = ");
			Serial.print((int) mx);
			Serial.print(" my = ");
			Serial.print((int) my);
			Serial.print(" mz = ");
			Serial.print((int) mz);
			Serial.println(" mG");

			Serial.println("Software quaternions:");
			Serial.print("q0 = ");
			Serial.print(q[0]);
			Serial.print(" qx = ");
			Serial.print(q[1]);
			Serial.print(" qy = ");
			Serial.print(q[2]);
			Serial.print(" qz = ");
			Serial.println(q[3]);
			Serial.println("Hardware quaternions:");
			Serial.print("Q0 = ");
			Serial.print(Quat[0]);
			Serial.print(" Qx = ");
			Serial.print(Quat[1]);
			Serial.print(" Qy = ");
			Serial.print(Quat[2]);
			Serial.print(" Qz = ");
			Serial.println(Quat[3]);
		}

// tempCount = readTempData();  // Read the gyro adc values
//    temperature = ((float) tempCount) / 333.87 + 21.0; // Gyro chip temperature in degrees Centigrade
		// Print temperature in degrees Centigrade
//    Serial.print("Gyro temperature is ");  Serial.print(temperature, 1);  Serial.println(" degrees C"); // Print T values to tenths of s degree C
		if (passThru) {
			rawPress = readBMP280Pressure();
			pressure = (float) bmp280_compensate_P(rawPress) / 25600.; // Pressure in mbar
			rawTemp = readBMP280Temperature();
			temperature = (float) bmp280_compensate_T(rawTemp) / 100.;

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
		yaw = atan2(2.0f * (q[1] * q[2] + q[0] * q[3]),
				q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]);
		pitch = -asin(2.0f * (q[1] * q[3] - q[0] * q[2]));
		roll = atan2(2.0f * (q[0] * q[1] + q[2] * q[3]),
				q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]);
		pitch *= 180.0f / PI;
		yaw *= 180.0f / PI;
		yaw += 13.8f; // Declination at Danville, California is 13 degrees 48 minutes and 47 seconds on 2014-04-04
		if (yaw < 0)
			yaw += 360.0f; // Ensure yaw stays between 0 and 360
		roll *= 180.0f / PI;
		//Hardware AHRS:
		Yaw = atan2(2.0f * (Quat[0] * Quat[1] + Quat[3] * Quat[2]),
				Quat[3] * Quat[3] + Quat[0] * Quat[0] - Quat[1] * Quat[1]
						- Quat[2] * Quat[2]);
		Pitch = -asin(2.0f * (Quat[0] * Quat[2] - Quat[3] * Quat[1]));
		Roll = atan2(2.0f * (Quat[3] * Quat[0] + Quat[1] * Quat[2]),
				Quat[3] * Quat[3] - Quat[0] * Quat[0] - Quat[1] * Quat[1]
						+ Quat[2] * Quat[2]);
		Pitch *= 180.0f / PI;
		Yaw *= 180.0f / PI;
		Yaw += 13.8f; // Declination at Danville, California is 13 degrees 48 minutes and 47 seconds on 2014-04-04
		if (Yaw < 0)
			Yaw += 360.0f; // Ensure yaw stays between 0 and 360
		Roll *= 180.0f / PI;

		// Or define output variable according to the Android system, where heading (0 to 360) is defined by the angle between the y-axis
		// and True North, pitch is rotation about the x-axis (-180 to +180), and roll is rotation about the y-axis (-90 to +90)
		// In this systen, the z-axis is pointing away from Earth, the +y-axis is at the "top" of the device (cellphone) and the +x-axis
		// points toward the right of the device.
		//

		if (SerialDebug) {
			Serial.print("Software yaw, pitch, roll: ");
			Serial.print(yaw, 2);
			Serial.print(", ");
			Serial.print(pitch, 2);
			Serial.print(", ");
			Serial.println(roll, 2);

			Serial.print("Hardware Yaw, Pitch, Roll: ");
			Serial.print(Yaw, 2);
			Serial.print(", ");
			Serial.print(Pitch, 2);
			Serial.print(", ");
			Serial.println(Roll, 2);

			Serial.println("BMP280:");
			Serial.print("Altimeter temperature = ");
			Serial.print(temperature, 2);
			Serial.println(" C"); // temperature in degrees Celsius
			Serial.print("Altimeter temperature = ");
			Serial.print(9. * temperature / 5. + 32., 2);
			Serial.println(" F"); // temperature in degrees Fahrenheit
			Serial.print("Altimeter pressure = ");
			Serial.print(pressure, 2);
			Serial.println(" mbar"); // pressure in millibar
			altitude = 145366.45f
					* (1.0f - pow((pressure / 1013.25f), 0.190284f));
			Serial.print("Altitude = ");
			Serial.print(altitude, 2);
			Serial.println(" feet");
			Serial.println(" ");
		}

		Serial.print("rate = ");
		Serial.print((float) sumCount / sum, 2);
		Serial.println(" Hz");
		//     Serial.print(millis()/1000.0, 1);Serial.print(",");
		//     Serial.print(yaw); Serial.print(",");Serial.print(pitch); Serial.print(",");Serial.print(roll); Serial.print(",");
		//     Serial.print(Yaw); Serial.print(",");Serial.print(Pitch); Serial.print(",");Serial.println(Roll);

		digitalWrite(LED, !digitalRead(LED));
		count = millis();
		sumCount = 0;
		sum = 0;
	}

}
