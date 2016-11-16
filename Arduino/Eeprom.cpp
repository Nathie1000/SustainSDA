// 
// 
// 

#include "Eeprom.h"
#include "ReadWriteByte.h"
#include "DefineAddress.h"

enum aScale {
	afs_2g = 0,
	afs_4g,
	afs_8g,
	afs_16g
};

enum gScale {
	gfs_250dps = 0,
	gfs_500dps,
	gfs_1000dps,
	gfs_2000dps
};

enum mScale {
	mfs_14bits = 0, // 0.6 mg per lsb
	mfs_16bits      // 0.15 mg per lsb
};

uint8_t gScale = gfs_250dps;
uint8_t aScale = afs_2g;
uint8_t mScale = mfs_16bits; // choose either 14-bit or 16-bit magnetometer resolution

Eeprom::Eeprom() {
}

Eeprom::~Eeprom() {

}

bool Eeprom::initEEPROM() {
	// check sentral status, make sure eeprom upload of firmware was accomplished
	byte stat;
	int count = 0;
	while (!stat) {
		ReadWriteByte::writeByte(EM7180_ADDRESS, EM7180_ResetRequest, 0x01);
		delay(500);
		count++;
		stat = (ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x01);
		if (ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x01) {
			//Serial.println("eeprom detected on the sensor bus!");
		}
		if (ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x02) {
			//Serial.println("eeprom uploaded config file!");
		}
		if (ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x04) {
			//Serial.println("eeprom crc incorrect!");
		}
		if (ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x08) {
			//Serial.println("em7180 in initialized state!");
		}
		if (ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x10) {
			//Serial.println("no eeprom detected!");
		}
		if (count > 10) break;
	}

	return (!(ReadWriteByte::readByte(EM7180_ADDRESS, EM7180_SentralStatus) & 0x04));  //Serial.println("eeprom upload successful!");
}

void Eeprom::initMPU9250(){
	// wake up device
	ReadWriteByte::writeByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x00); // clear sleep mode bit (6), enable all sensors 
	delay(100); // wait for all registers to reset 

				// get stable time source
	ReadWriteByte::writeByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x01);  // auto select clock source to be pll gyroscope reference if ready else
	delay(200);

	// configure gyro and thermometer
	// disable fsync and set thermometer and gyro bandwidth to 41 and 42 hz, respectively; 
	// minimum delay time for this setting is 5.9 ms, which means sensor fusion update rates cannot
	// be higher than 1 / 0.0059 = 170 hz
	// dlpf_cfg = bits 2:0 = 011; this limits the sample rate to 1000 hz for both
	// with the mpu9250, it is possible to get gyro sample rates of 32 khz (!), 8 khz, or 1 khz
	ReadWriteByte::writeByte(MPU9250_ADDRESS, CONFIG, 0x03);

	// set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
	ReadWriteByte::writeByte(MPU9250_ADDRESS, SMPLRT_DIV, 0x04);  // use a 200 hz rate; a rate consistent with the filter update rate 
												   // determined inset in config above

												   // set gyroscope full scale range
												   // range selects fs_sel and afs_sel are 0 - 3, so 2-bit values are left-shifted into positions 4:3
	uint8_t c = ReadWriteByte::readByte(MPU9250_ADDRESS, GYRO_CONFIG); // get current GYRO_CONFIG register value
														// c = c & ~0xe0; // clear self-test bits [7:5] 
	c = c & ~0x02; // clear fchoice bits [1:0] 
	c = c & ~0x18; // clear afs bits [4:3]
	c = c | gScale << 3; // set full scale range for the gyro
						 // c =| 0x00; // set fchoice for the gyro to 11 by writing its inverse to bits 1:0 of GYRO_CONFIG
	ReadWriteByte::writeByte(MPU9250_ADDRESS, GYRO_CONFIG, c); // write new GYRO_CONFIG value to register

												// set accelerometer full-scale range configuration
	c = ReadWriteByte::readByte(MPU9250_ADDRESS, ACCEL_CONFIG); // get current ACCEL_CONFIG register value
												 // c = c & ~0xe0; // clear self-test bits [7:5] 
	c = c & ~0x18;  // clear afs bits [4:3]
	c = c | aScale << 3; // set full scale range for the accelerometer 
	ReadWriteByte::writeByte(MPU9250_ADDRESS, ACCEL_CONFIG, c); // write new ACCEL_CONFIG register value

												 // set accelerometer sample rate configuration
												 // it is possible to get a 4 khz sample rate from the accelerometer by choosing 1 for
												 // accel_fchoice_b bit [3]; in this case the bandwidth is 1.13 khz
	c = ReadWriteByte::readByte(MPU9250_ADDRESS, ACCEL_CONFIG2); // get current ACCEL_CONFIG2 register value
	c = c & ~0x0f; // clear accel_fchoice_b (bit 3) and a_dlpfg (bits [2:0])  
	c = c | 0x03;  // set accelerometer rate to 1 khz and bandwidth to 41 hz
	ReadWriteByte::writeByte(MPU9250_ADDRESS, ACCEL_CONFIG2, c); // write new ACCEL_CONFIG2 register value

												  // the accelerometer, gyro, and thermometer are set to 1 khz sample rates, 
												  // but all these rates are further reduced by a factor of 5 to 200 hz because of the SMPLRT_DIV setting

												  // configure interrupts and bypass enable
												  // set interrupt pin active high, push-pull, hold interrupt pin level high until interrupt cleared,
												  // clear on read of int_status, and enable i2c_bypass_en so additional chips 
												  // can join the i2c bus and all can be controlled by the arduino as master
	ReadWriteByte::writeByte(MPU9250_ADDRESS, INT_PIN_CFG, 0x22);
	ReadWriteByte::writeByte(MPU9250_ADDRESS, INT_ENABLE, 0x01);  // enable data ready (bit 0) interrupt
}

void Eeprom::initAK8963(float * destination)
{
	// first extract the factory calibration for each magnetometer axis
	uint8_t rawdata[3];  // x/y/z gyro calibration data stored here
	ReadWriteByte::writeByte(AK8963_ADDRESS, AK8963_CNTL, 0x00); // power down magnetometer  
	delay(20);
	ReadWriteByte::writeByte(AK8963_ADDRESS, AK8963_CNTL, 0x0f); // enter fuse rom access mode
	delay(20);
	ReadWriteByte::readBytes(AK8963_ADDRESS, AK8963_ASAX, 3, &rawdata[0]);  // read the x-, y-, and z-axis calibration values
	destination[0] = (float)(rawdata[0] - 128) / 256. + 1.;   // return x-axis sensitivity adjustment values, etc.
	destination[1] = (float)(rawdata[1] - 128) / 256. + 1.;
	destination[2] = (float)(rawdata[2] - 128) / 256. + 1.;
	ReadWriteByte::writeByte(AK8963_ADDRESS, AK8963_CNTL, 0x00); // power down magnetometer  
	delay(20);
	// configure the magnetometer for continuous read and highest resolution
	// set mscale bit 4 to 1 (0) to enable 16 (14) bit resolution in cntl register,
	// and enable continuous mode data acquisition mmode (bits [3:0]), 0010 for 8 hz and 0110 for 100 hz sample rates
	ReadWriteByte::writeByte(AK8963_ADDRESS, AK8963_CNTL, mScale << 4 | mMode); // set magnetometer data resolution and sample odr
}

// function which accumulates gyro and accelerometer data after device initialization. it calculates the average
// of the at-rest readings and then loads the resulting offsets into accelerometer and gyro bias registers.
void Eeprom::magcalMPU9250(float * dest1, float * dest2)
{
	uint16_t sample_count = 0;
	int32_t mag_bias[3] = { 0, 0, 0 }, mag_scale[3] = { 0, 0, 0 };
	int16_t mag_max[3] = { 0xff, 0xff, 0xff }, mag_min[3] = { 0x7f, 0x7f, 0x7f }, mag_temp[3] = { 0, 0, 0 };

	Serial.println("mag calibration: wave device in a figure eight until done!");
	delay(4000);

	if (mMode == 0x02) { sample_count = 128; }
	else if (mMode == 0x06) { sample_count = 1500; }
	for (uint16_t ii = 0; ii < sample_count; ii++) {
		readMagData(mag_temp);  // read the mag data   
		for (int jj = 0; jj < 3; jj++) {
			if (mag_temp[jj] > mag_max[jj]) mag_max[jj] = mag_temp[jj];
			if (mag_temp[jj] < mag_min[jj]) mag_min[jj] = mag_temp[jj];
		}
		if (mMode == 0x02) { delay(135); }  // at 8 hz odr, new mag data is available every 125 ms
		else if (mMode == 0x06) { delay(12); } // at 100 hz odr, new mag data is available every 10 ms
	}

	// get hard iron correction
	mag_bias[0] = (mag_max[0] + mag_min[0]) / 2;  // get average x mag bias in counts
	mag_bias[1] = (mag_max[1] + mag_min[1]) / 2;  // get average y mag bias in counts
	mag_bias[2] = (mag_max[2] + mag_min[2]) / 2;  // get average z mag bias in counts

	dest1[0] = (float)mag_bias[0] * mRes*magCalibration[0];  // save mag biases in g for main program
	dest1[1] = (float)mag_bias[1] * mRes*magCalibration[1];
	dest1[2] = (float)mag_bias[2] * mRes*magCalibration[2];

	// get soft iron correction estimate
	mag_scale[0] = (mag_max[0] - mag_min[0]) / 2;  // get average x axis max chord length in counts
	mag_scale[1] = (mag_max[1] - mag_min[1]) / 2;  // get average y axis max chord length in counts
	mag_scale[2] = (mag_max[2] - mag_min[2]) / 2;  // get average z axis max chord length in counts

	float avg_rad = mag_scale[0] + mag_scale[1] + mag_scale[2];
	avg_rad /= 3.0;

	dest2[0] = avg_rad / ((float)mag_scale[0]);
	dest2[1] = avg_rad / ((float)mag_scale[1]);
	dest2[2] = avg_rad / ((float)mag_scale[2]);

	Serial.println("mag calibration done!");
}

void Eeprom::getAverageValue(uint8_t rawdata[], int16_t deviceaverage[], int16_t deviceaverage2[]) {
	for (int ii = 0; ii < 200; ii++) {  // get average current values of gyro and acclerometer

		ReadWriteByte::readBytes(MPU9250_ADDRESS, ACCEL_XOUT_H, 6, &rawdata[0]);        // read the six raw data registers into data array
		deviceaverage[0] += (int16_t)(((int16_t)rawdata[0] << 8) | rawdata[1]);  // turn the msb and lsb into a signed 16-bit value
		deviceaverage[1] += (int16_t)(((int16_t)rawdata[2] << 8) | rawdata[3]);
		deviceaverage[2] += (int16_t)(((int16_t)rawdata[4] << 8) | rawdata[5]);

		ReadWriteByte::readBytes(MPU9250_ADDRESS, GYRO_XOUT_H, 6, &rawdata[0]);       // read the six raw data registers sequentially into data array
		deviceaverage2[0] += (int16_t)(((int16_t)rawdata[0] << 8) | rawdata[1]);  // turn the msb and lsb into a signed 16-bit value
		deviceaverage2[1] += (int16_t)(((int16_t)rawdata[2] << 8) | rawdata[3]);
		deviceaverage2[2] += (int16_t)(((int16_t)rawdata[4] << 8) | rawdata[5]);
	}

	for (int ii = 0; ii < 3; ii++) {  // get average of 200 values and store as average current readings
		deviceaverage[ii] /= 200;
		deviceaverage2[ii] /= 200;
	}
}

// accelerometer and gyroscope self test; check calibration wrt factory settings
void Eeprom::MPU9250SelfTest(float * destination) // should return percent deviation from factory trim values, +/- 14 or less deviation is a pass
{
	uint8_t rawdata[6] = { 0, 0, 0, 0, 0, 0 };
	uint8_t selftest[6];
	int16_t gavg[3], aavg[3], astavg[3], gstavg[3];
	float factorytrim[6];
	uint8_t fs = 0;

	ReadWriteByte::writeByte(MPU9250_ADDRESS, SMPLRT_DIV, 0x00);    // set gyro sample rate to 1 khz
	ReadWriteByte::writeByte(MPU9250_ADDRESS, CONFIG, 0x02);        // set gyro sample rate to 1 khz and dlpf to 92 hz
	ReadWriteByte::writeByte(MPU9250_ADDRESS, GYRO_CONFIG, 1 << fs);  // set full scale range for the gyro to 250 dps
	ReadWriteByte::writeByte(MPU9250_ADDRESS, ACCEL_CONFIG2, 0x02); // set accelerometer rate to 1 khz and bandwidth to 92 hz
	ReadWriteByte::writeByte(MPU9250_ADDRESS, ACCEL_CONFIG, 1 << fs); // set full scale range for the accelerometer to 2 g

	getAverageValue(rawdata, aavg, gavg);

	// configure the accelerometer for self-test
	ReadWriteByte::writeByte(MPU9250_ADDRESS, ACCEL_CONFIG, 0xe0); // enable self test on all three axes and set accelerometer range to +/- 2 g
	ReadWriteByte::writeByte(MPU9250_ADDRESS, GYRO_CONFIG, 0xe0); // enable self test on all three axes and set gyro range to +/- 250 degrees/s
	delay(25);  // delay a while to let the device stabilize

	getAverageValue(rawdata, astavg, gstavg);

	// configure the gyro and accelerometer for normal operation
	ReadWriteByte::writeByte(MPU9250_ADDRESS, ACCEL_CONFIG, 0x00);
	ReadWriteByte::writeByte(MPU9250_ADDRESS, GYRO_CONFIG, 0x00);
	delay(25);  // delay a while to let the device stabilize

	// retrieve accelerometer and gyro factory self-test code from usr_reg
	selftest[0] = ReadWriteByte::readByte(MPU9250_ADDRESS, SELF_TEST_X_ACCEL); // x-axis accel self-test results
	selftest[1] = ReadWriteByte::readByte(MPU9250_ADDRESS, SELF_TEST_Y_ACCEL); // y-axis accel self-test results
	selftest[2] = ReadWriteByte::readByte(MPU9250_ADDRESS, SELF_TEST_Z_ACCEL); // z-axis accel self-test results
	selftest[3] = ReadWriteByte::readByte(MPU9250_ADDRESS, SELF_TEST_X_GYRO);  // x-axis gyro self-test results
	selftest[4] = ReadWriteByte::readByte(MPU9250_ADDRESS, SELF_TEST_Y_GYRO);  // y-axis gyro self-test results
	selftest[5] = ReadWriteByte::readByte(MPU9250_ADDRESS, SELF_TEST_Z_GYRO);  // z-axis gyro self-test results

																// retrieve factory self-test value from self-test code reads
	factorytrim[0] = (float)(2620 / 1 << fs)*(pow(1.01, ((float)selftest[0] - 1.0))); // ft[xa] factory trim calculation
	factorytrim[1] = (float)(2620 / 1 << fs)*(pow(1.01, ((float)selftest[1] - 1.0))); // ft[ya] factory trim calculation
	factorytrim[2] = (float)(2620 / 1 << fs)*(pow(1.01, ((float)selftest[2] - 1.0))); // ft[za] factory trim calculation
	factorytrim[3] = (float)(2620 / 1 << fs)*(pow(1.01, ((float)selftest[3] - 1.0))); // ft[xg] factory trim calculation
	factorytrim[4] = (float)(2620 / 1 << fs)*(pow(1.01, ((float)selftest[4] - 1.0))); // ft[yg] factory trim calculation
	factorytrim[5] = (float)(2620 / 1 << fs)*(pow(1.01, ((float)selftest[5] - 1.0))); // ft[zg] factory trim calculation

																					  // report results as a ratio of (str - ft)/ft; the change from factory trim of the self-test response
																					  // to get percent, must multiply by 100
	for (int i = 0; i < 3; i++) {
		destination[i] = 100.0*((float)(astavg[i] - aavg[i])) / factorytrim[i];   // report percent differences
		destination[i + 3] = 100.0*((float)(gstavg[i] - gavg[i])) / factorytrim[i + 3]; // report percent differences
	}

}

void Eeprom::getMagResult() {
	switch (mScale) {
		// possible magnetometer scales (and their register bit settings) are:
		// 14 bit resolution (0) and 16 bit resolution (1)
	case mfs_14bits:
		mRes = 10.*4912. / 8190.; // proper scale to return milligauss
		break;
	case mfs_16bits:
		mRes = 10.*4912. / 32760.0; // proper scale to return milligauss
		break;
	}
}

void Eeprom::getGyroResult() {
	switch (gScale) {
		// possible gyro scales (and their register bit settings) are:
		// 250 dps (00), 500 dps (01), 1000 dps (10), and 2000 dps  (11). 
		// here's a bit of an algorith to calculate dps/(adc tick) based on that 2-bit value:
	case gfs_250dps:
		gRes = 250.0 / 32768.0;
		break;
	case gfs_500dps:
		gRes = 500.0 / 32768.0;
		break;
	case gfs_1000dps:
		gRes = 1000.0 / 32768.0;
		break;
	case gfs_2000dps:
		gRes = 2000.0 / 32768.0;
		break;
	}
}

void Eeprom::getAccelResult() {
	switch (aScale) {
		// possible accelerometer scales (and their register bit settings) are:
		// 2 gs (00), 4 gs (01), 8 gs (10), and 16 gs  (11). 
		// here's a bit of an algorith to calculate dps/(adc tick) based on that 2-bit value:
	case afs_2g:
		aRes = 2.0 / 32768.0;
		break;
	case afs_4g:
		aRes = 4.0 / 32768.0;
		break;
	case afs_8g:
		aRes = 8.0 / 32768.0;
		break;
	case afs_16g:
		aRes = 16.0 / 32768.0;
		break;
	}
}

void Eeprom::accelGyroCalMPU9250(float * dest1, float * dest2)
{
	uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
	uint16_t ii, packet_count, fifo_count;
	int32_t gyro_bias[3] = { 0, 0, 0 }, accel_bias[3] = { 0, 0, 0 };

	// reset device
	ReadWriteByte::writeByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x80); // write a one to bit 7 reset bit; toggle reset device
	delay(100);

	// get stable time source; auto select clock source to be pll gyroscope reference if ready 
	// else use the internal oscillator, bits 2:0 = 001
	ReadWriteByte::writeByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x01);
	ReadWriteByte::writeByte(MPU9250_ADDRESS, PWR_MGMT_2, 0x00);
	delay(200);

	// configure device for bias calculation
	ReadWriteByte::writeByte(MPU9250_ADDRESS, INT_ENABLE, 0x00);   // disable all interrupts
	ReadWriteByte::writeByte(MPU9250_ADDRESS, FIFO_EN, 0x00);      // disable fifo
	ReadWriteByte::writeByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x00);   // turn on internal clock source
	ReadWriteByte::writeByte(MPU9250_ADDRESS, I2C_MST_CTRL, 0x00); // disable i2c master
	ReadWriteByte::writeByte(MPU9250_ADDRESS, USER_CTRL, 0x00);    // disable fifo and i2c master modes
	ReadWriteByte::writeByte(MPU9250_ADDRESS, USER_CTRL, 0x0c);    // reset fifo and dmp
	delay(15);

	// configure mpu6050 gyro and accelerometer for bias calculation
	ReadWriteByte::writeByte(MPU9250_ADDRESS, CONFIG, 0x01);      // set low-pass filter to 188 hz
	ReadWriteByte::writeByte(MPU9250_ADDRESS, SMPLRT_DIV, 0x00);  // set sample rate to 1 khz
	ReadWriteByte::writeByte(MPU9250_ADDRESS, GYRO_CONFIG, 0x00);  // set gyro full-scale to 250 degrees per second, maximum sensitivity
	ReadWriteByte::writeByte(MPU9250_ADDRESS, ACCEL_CONFIG, 0x00); // set accelerometer full-scale to 2 g, maximum sensitivity

	uint16_t  gyrosensitivity = 131;   // = 131 lsb/degrees/sec
	uint16_t  accelsensitivity = 16384;  // = 16384 lsb/g

										 // configure fifo to capture accelerometer and gyro data for bias calculation
	ReadWriteByte::writeByte(MPU9250_ADDRESS, USER_CTRL, 0x40);   // enable fifo  
	ReadWriteByte::writeByte(MPU9250_ADDRESS, FIFO_EN, 0x78);     // enable gyro and accelerometer sensors for fifo  (max size 512 bytes in mpu-9150)
	delay(40); // accumulate 40 samples in 40 milliseconds = 480 bytes

			   // at end of sample accumulation, turn off fifo sensor read
	ReadWriteByte::writeByte(MPU9250_ADDRESS, FIFO_EN, 0x00);        // disable gyro and accelerometer sensors for fifo
	ReadWriteByte::readBytes(MPU9250_ADDRESS, FIFO_COUNTH, 2, &data[0]); // read fifo sample count
	fifo_count = ((uint16_t)data[0] << 8) | data[1];
	packet_count = fifo_count / 12;// how many sets of full gyro and accelerometer data for averaging

	for (ii = 0; ii < packet_count; ii++) {
		int16_t accel_temp[3] = { 0, 0, 0 }, gyro_temp[3] = { 0, 0, 0 };
		ReadWriteByte::readBytes(MPU9250_ADDRESS, FIFO_R_W, 12, &data[0]); // read data for averaging
		accel_temp[0] = (int16_t)(((int16_t)data[0] << 8) | data[1]);  // form signed 16-bit integer for each sample in fifo
		accel_temp[1] = (int16_t)(((int16_t)data[2] << 8) | data[3]);
		accel_temp[2] = (int16_t)(((int16_t)data[4] << 8) | data[5]);
		gyro_temp[0] = (int16_t)(((int16_t)data[6] << 8) | data[7]);
		gyro_temp[1] = (int16_t)(((int16_t)data[8] << 8) | data[9]);
		gyro_temp[2] = (int16_t)(((int16_t)data[10] << 8) | data[11]);

		accel_bias[0] += (int32_t)accel_temp[0]; // sum individual signed 16-bit biases to get accumulated signed 32-bit biases
		accel_bias[1] += (int32_t)accel_temp[1];
		accel_bias[2] += (int32_t)accel_temp[2];
		gyro_bias[0] += (int32_t)gyro_temp[0];
		gyro_bias[1] += (int32_t)gyro_temp[1];
		gyro_bias[2] += (int32_t)gyro_temp[2];

	}
	accel_bias[0] /= (int32_t)packet_count; // normalize sums to get average count biases
	accel_bias[1] /= (int32_t)packet_count;
	accel_bias[2] /= (int32_t)packet_count;
	gyro_bias[0] /= (int32_t)packet_count;
	gyro_bias[1] /= (int32_t)packet_count;
	gyro_bias[2] /= (int32_t)packet_count;

	if (accel_bias[2] > 0l) { accel_bias[2] -= (int32_t)accelsensitivity; }  // remove gravity from the z-axis accelerometer bias calculation
	else { accel_bias[2] += (int32_t)accelsensitivity; }

	// construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
	data[0] = (-gyro_bias[0] / 4 >> 8) & 0xff; // divide by 4 to get 32.9 lsb per deg/s to conform to expected bias input format
	data[1] = (-gyro_bias[0] / 4) & 0xff; // biases are additive, so change sign on calculated average gyro biases
	data[2] = (-gyro_bias[1] / 4 >> 8) & 0xff;
	data[3] = (-gyro_bias[1] / 4) & 0xff;
	data[4] = (-gyro_bias[2] / 4 >> 8) & 0xff;
	data[5] = (-gyro_bias[2] / 4) & 0xff;

	// push gyro biases to hardware registers
	ReadWriteByte::writeByte(MPU9250_ADDRESS, XG_OFFSET_H, data[0]);
	ReadWriteByte::writeByte(MPU9250_ADDRESS, XG_OFFSET_L, data[1]);
	ReadWriteByte::writeByte(MPU9250_ADDRESS, YG_OFFSET_H, data[2]);
	ReadWriteByte::writeByte(MPU9250_ADDRESS, YG_OFFSET_L, data[3]);
	ReadWriteByte::writeByte(MPU9250_ADDRESS, ZG_OFFSET_H, data[4]);
	ReadWriteByte::writeByte(MPU9250_ADDRESS, ZG_OFFSET_L, data[5]);

	// output scaled gyro biases for display in the main program
	dest1[0] = (float)gyro_bias[0] / (float)gyrosensitivity;
	dest1[1] = (float)gyro_bias[1] / (float)gyrosensitivity;
	dest1[2] = (float)gyro_bias[2] / (float)gyrosensitivity;

	// construct the accelerometer biases for push to the hardware accelerometer bias registers. these registers contain
	// factory trim values which must be added to the calculated accelerometer biases; on boot up these registers will hold
	// non-zero values. in addition, bit 0 of the lower byte must be preserved since it is used for temperature
	// compensation calculations. accelerometer bias registers expect bias input as 2048 lsb per g, so that
	// the accelerometer biases calculated above must be divided by 8.

	int32_t accel_bias_reg[3] = { 0, 0, 0 }; // a place to hold the factory accelerometer trim biases
	ReadWriteByte::readBytes(MPU9250_ADDRESS, XA_OFFSET_H, 2, &data[0]); // read factory accelerometer trim values
	accel_bias_reg[0] = (int32_t)(((int16_t)data[0] << 8) | data[1]);
	ReadWriteByte::readBytes(MPU9250_ADDRESS, YA_OFFSET_H, 2, &data[0]);
	accel_bias_reg[1] = (int32_t)(((int16_t)data[0] << 8) | data[1]);
	ReadWriteByte::readBytes(MPU9250_ADDRESS, ZA_OFFSET_H, 2, &data[0]);
	accel_bias_reg[2] = (int32_t)(((int16_t)data[0] << 8) | data[1]);

	uint32_t mask = 1ul; // define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers
	uint8_t mask_bit[3] = { 0, 0, 0 }; // define array to hold mask bit for each accelerometer bias axis

	for (ii = 0; ii < 3; ii++) {
		if ((accel_bias_reg[ii] & mask)) mask_bit[ii] = 0x01; // if temperature compensation bit is set, record that fact in mask_bit
	}

	// construct total accelerometer bias, including calculated average accelerometer bias from above
	accel_bias_reg[0] -= (accel_bias[0] / 8); // subtract calculated averaged accelerometer bias scaled to 2048 lsb/g (16 g full scale)
	accel_bias_reg[1] -= (accel_bias[1] / 8);
	accel_bias_reg[2] -= (accel_bias[2] / 8);

	data[0] = (accel_bias_reg[0] >> 8) & 0xfe;
	data[1] = (accel_bias_reg[0]) & 0xfe;
	data[1] = data[1] | mask_bit[0]; // preserve temperature compensation bit when writing back to accelerometer bias registers
	data[2] = (accel_bias_reg[1] >> 8) & 0xfe;
	data[3] = (accel_bias_reg[1]) & 0xfe;
	data[3] = data[3] | mask_bit[1]; // preserve temperature compensation bit when writing back to accelerometer bias registers
	data[4] = (accel_bias_reg[2] >> 8) & 0xfe;
	data[5] = (accel_bias_reg[2]) & 0xfe;
	data[5] = data[5] | mask_bit[2]; // preserve temperature compensation bit when writing back to accelerometer bias registers

									 // apparently this is not working for the acceleration biases in the mpu-9250
									 // are we handling the temperature correction bit properly?
									 // push accelerometer biases to hardware registers
									 /*  writeByte(mpu9250_address, XA_OFFSET_H, data[0]);
									 writeByte(mpu9250_address, xa_offset_l, data[1]);
									 writeByte(mpu9250_address, YA_OFFSET_H, data[2]);
									 writeByte(mpu9250_address, ya_offset_l, data[3]);
									 writeByte(mpu9250_address, ZA_OFFSET_H, data[4]);
									 writeByte(mpu9250_address, za_offset_l, data[5]);
									 */
									 // output scaled accelerometer biases for display in the main program
	dest2[0] = (float)accel_bias[0] / (float)accelsensitivity;
	dest2[1] = (float)accel_bias[1] / (float)accelsensitivity;
	dest2[2] = (float)accel_bias[2] / (float)accelsensitivity;
}

void Eeprom::readMagData(int16_t * destination)
{
	uint8_t rawdata[7];  // x/y/z gyro register data, st2 register stored here, must read st2 at end of data acquisition
	if (ReadWriteByte::readByte(AK8963_ADDRESS, AK8963_ST1) & 0x01) { // wait for magnetometer data ready bit to be set
		ReadWriteByte::readBytes(AK8963_ADDRESS, AK8963_XOUT_L, 7, &rawdata[0]);  // read the six raw data and st2 registers sequentially into data array
		uint8_t c = rawdata[6]; // end data read by reading st2 register
		if (!(c & 0x08)) { // check if magnetic sensor overflow set, if not then report data
			destination[0] = ((int16_t)rawdata[1] << 8) | rawdata[0];  // turn the msb and lsb into a signed 16-bit value
			destination[1] = ((int16_t)rawdata[3] << 8) | rawdata[2];  // data stored as little endian
			destination[2] = ((int16_t)rawdata[5] << 8) | rawdata[4];
		}
	}
}

void Eeprom::readData(int16_t * destination, int deviceAddress)
{
	uint8_t rawdata[6];  // x/y/z accel register data stored here
	ReadWriteByte::readBytes(MPU9250_ADDRESS, deviceAddress, 6, &rawdata[0]);  // read the six raw data registers into data array
	destination[0] = ((int16_t)rawdata[0] << 8) | rawdata[1];  // turn the msb and lsb into a signed 16-bit value
	destination[1] = ((int16_t)rawdata[2] << 8) | rawdata[3];
	destination[2] = ((int16_t)rawdata[4] << 8) | rawdata[5];
}

// implementation of sebastian madgwick's "...efficient orientation filter for... inertial/magnetic sensor arrays"
// (see http://www.x-io.co.uk/category/open-source/ for examples and more details)
// which fuses acceleration, rotation rate, and magnetic moments to produce a quaternion-based estimate of absolute
// device orientation -- which can be converted to yaw, pitch, and roll. useful for stabilizing quadcopters, etc.
// the performance of the orientation filter is at least as good as conventional kalman-based filtering algorithms
// but is much less computationally intensive---it can be performed on a 3.3 v pro mini operating at 8 mhz!
void Eeprom::MadgwickQuaternionUpdate(float accelX, float accelY, float accelZ, float gyroX, float gyroY, float gyroZ, float magX, float magY, float magZ, float deltat)
{
	float q1 = q[0], q2 = q[1], q3 = q[2], q4 = q[3];   // short name local variable for readability

	// auxiliary variables to avoid repeated arithmetic
	float _2q1 = 2.0f * q1;
	float _2q2 = 2.0f * q2;
	float _2q3 = 2.0f * q3;
	float _2q4 = 2.0f * q4;
	float _2q1q3 = 2.0f * q1 * q3;
	float _2q3q4 = 2.0f * q3 * q4;
	float q1q1 = q1 * q1;
	float q1q2 = q1 * q2;
	float q1q3 = q1 * q3;
	float q1q4 = q1 * q4;
	float q2q2 = q2 * q2;
	float q2q3 = q2 * q3;
	float q2q4 = q2 * q4;
	float q3q3 = q3 * q3;
	float q3q4 = q3 * q4;
	float q4q4 = q4 * q4;

	// normalise accelerometer measurement
	float norm = sqrtf(accelX * accelX + accelY * accelY + accelZ * accelZ);
	if (norm == 0.0f) return; // handle nan
	norm = 1.0f / norm;
	accelX *= norm;
	accelY *= norm;
	accelZ *= norm;

	// normalise magnetometer measurement
	norm = sqrtf(magX * magX + magY * magY + magZ * magZ);
	if (norm == 0.0f) return; // handle nan
	norm = 1.0f / norm;
	magX *= norm;
	magY *= norm;
	magZ *= norm;

	// reference direction of earth's magnetic field
	float hx = magX * q1q1 - 2.0f * q1 * magY * q4 + 2.0f * q1 * magZ * q3 + magX * q2q2 + _2q2 * magY * q3 + _2q2 * magZ * q4 - magX * q3q3 - magX * q4q4;
	float hy = 2.0f * q1 * magX * q4 + magY * q1q1 - 2.0f * q1 * magZ * q2 + 2.0f * q2 * magX * q3 - magY * q2q2 + magY * q3q3 + _2q3 * magZ * q4 - magY * q4q4;
	float _2bx = sqrtf(hx * hx + hy * hy);
	float _2bz = -2.0f * q1 * magX * q3 + 2.0f * q1 * magY * q2 + magZ * q1q1 + 2.0f * q2 * magX * q4 - magZ * q2q2 + _2q3 * magY * q4 - magZ * q3q3 + magZ * q4q4;
	float _4bx = 2.0f * _2bx;
	float _4bz = 2.0f * _2bz;

	// gradient decent algorithm corrective step
	float s1 = -_2q3 * (2.0f * q2q4 - _2q1q3 - accelX) + _2q2 * (2.0f * q1q2 + _2q3q4 - accelY) - _2bz * q3 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - magX) + (-_2bx * q4 + _2bz * q2) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - magY) + _2bx * q3 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - magZ);
	float s2 = _2q4 * (2.0f * q2q4 - _2q1q3 - accelX) + _2q1 * (2.0f * q1q2 + _2q3q4 - accelY) - 4.0f * q2 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - accelZ) + _2bz * q4 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - magX) + (_2bx * q3 + _2bz * q1) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - magY) + (_2bx * q4 - _4bz * q2) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - magZ);
	float s3 = -_2q1 * (2.0f * q2q4 - _2q1q3 - accelX) + _2q4 * (2.0f * q1q2 + _2q3q4 - accelY) - 4.0f * q3 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - accelZ) + (-_4bx * q3 - _2bz * q1) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - magX) + (_2bx * q2 + _2bz * q4) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - magY) + (_2bx * q1 - _4bz * q3) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - magZ);
	float s4 = _2q2 * (2.0f * q2q4 - _2q1q3 - accelX) + _2q3 * (2.0f * q1q2 + _2q3q4 - accelY) + (-_4bx * q4 + _2bz * q2) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - magX) + (-_2bx * q1 + _2bz * q3) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - magY) + _2bx * q2 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - magZ);
	norm = sqrtf(s1 * s1 + s2 * s2 + s3 * s3 + s4 * s4);    // normalise step magnitude
	norm = 1.0f / norm;
	s1 *= norm;
	s2 *= norm;
	s3 *= norm;
	s4 *= norm;

	// compute rate of change of quaternion
	float gyromeaserror = PI * (40.0f / 180.0f);   // gyroscope measurement error in rads/s (start at 40 deg/s)
	float beta = sqrt(3.0f / 4.0f) * gyromeaserror;   // compute beta


	float qdot1 = 0.5f * (-q2 * gyroX - q3 * gyroY - q4 * gyroZ) - beta * s1;
	float qdot2 = 0.5f * (q1 * gyroX + q3 * gyroZ - q4 * gyroY) - beta * s2;
	float qdot3 = 0.5f * (q1 * gyroY - q2 * gyroZ + q4 * gyroX) - beta * s3;
	float qdot4 = 0.5f * (q1 * gyroZ + q2 * gyroY - q3 * gyroX) - beta * s4;

	// integrate to yield quaternion
	q1 += qdot1 * deltat;
	q2 += qdot2 * deltat;
	q3 += qdot3 * deltat;
	q4 += qdot4 * deltat;
	norm = sqrtf(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);    // normalise quaternion
	norm = 1.0f / norm;
	q[0] = q1 * norm;
	q[1] = q2 * norm;
	q[2] = q3 * norm;
	q[3] = q4 * norm;

}

float Eeprom::calculateSoftwareYPR(char rotation) {
	if (rotation == 'Y' || rotation == 'y') {
		float softwareYaw = atan2(2.0f * (q[1] * q[2] + q[0] * q[3]), q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]);
    softwareYaw *= 180.0f / PI;
    softwareYaw += 13.8f; 

    return softwareYaw;
	}
	else if (rotation == 'P' || rotation == 'p') {
	float softwarePitch = -asin(2.0f * (q[1] * q[3] - q[0] * q[2]));
   softwarePitch *= 180.0f / PI;
   return softwarePitch;
	}
	else if (rotation == 'R' || rotation == 'r') {
		float softwareRoll = atan2(2.0f * (q[0] * q[1] + q[2] * q[3]), q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]);
    softwareRoll *= 180.0f / PI;
    return softwareRoll;
	}
  return 0.0f;
}

float Eeprom::calculateHardwareYPR(char rotation, float Quat[]){
  if(rotation == 'Y' || rotation == 'y') {
    float hardwareYaw = atan2(2.0f * (Quat[0] * Quat[1] + Quat[3] * Quat[2]), Quat[3] * Quat[3] + Quat[0] * Quat[0] - Quat[1] * Quat[1] - Quat[2] * Quat[2]);   
    hardwareYaw *= 180.0f / PI;
    hardwareYaw += 13.8f;
    return hardwareYaw;
  }
  else if(rotation == 'P' || rotation == 'p') {
    float hardwarePitch = -asin(2.0f * (Quat[0] * Quat[2] - Quat[3] * Quat[1]));
    hardwarePitch *= 180.0f / PI;
    return hardwarePitch;
  }
  else if(rotation == 'R' || rotation == 'r') {
    float hardwareRoll = atan2(2.0f * (Quat[3] * Quat[0] + Quat[1] * Quat[2]), Quat[3] * Quat[3] - Quat[0] * Quat[0] - Quat[1] * Quat[1] + Quat[2] * Quat[2]);
    hardwareRoll *= 180.0f / PI;
    return hardwareRoll;
  }
  return 0.0f;
}


