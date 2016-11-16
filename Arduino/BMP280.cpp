// 
// 
// 

#include "BMP280.h"

	enum posr {
		p_osr_00 = 0,  // no op
		p_osr_01,
		p_osr_02,
		p_osr_04,
		p_osr_08,
		p_osr_16
	};

	enum tosr {
		t_osr_00 = 0,  // no op
		t_osr_01,
		t_osr_02,
		t_osr_04,
		t_osr_08,
		t_osr_16
	};

	enum iirfilter {
		full = 0,  // bandwidth at full sample rate
		bw0_223odr,
		bw0_092odr,
		bw0_042odr,
		bw0_021odr // bandwidth at 0.021 x sample rate
	};

	enum mode {
		bmp280sleep = 0,
		forced,
		forced2,
		normal
	};

	enum sby {
		t_00_5ms = 0,
		t_62_5ms,
		t_125ms,
		t_250ms,
		t_500ms,
		t_1000ms,
		t_2000ms,
		t_4000ms,
	};

	// specify bmp280 configuration
	uint8_t posr = p_osr_16, tosr = t_osr_02, mode = normal, iirfilter = bw0_042odr, sby = t_62_5ms;     // set pressure amd temperature output data rate
	int32_t t_fine; // t_fine carries fine temperature as global value for bmp280

	// bmp280 compensation parameters
	uint16_t dig_t1, dig_p1;
	int16_t  dig_t2, dig_t3, dig_p2, dig_p3, dig_p4, dig_p5, dig_p6, dig_p7, dig_p8, dig_p9;

	BMP280::BMP280(){
	}

	BMP280::~BMP280() {

	}

	void BMP280::BMP280Init()
	{
		// configure the bmp280
		// set t and p oversampling rates and sensor mode
		ReadWriteByte::writeByte(BMP280_ADDRESS, BMP280_CTRL_MEAS, tosr << 5 | posr << 2 | mode);
		// set standby time interval in normal mode and bandwidth
		ReadWriteByte::writeByte(BMP280_ADDRESS, BMP280_CONFIG, sby << 5 | iirfilter << 2);
		// read and store calibration data
		uint8_t calib[24];
		ReadWriteByte::readBytes(BMP280_ADDRESS, BMP280_CALIB00, 24, &calib[0]);
		dig_t1 = (uint16_t)(((uint16_t)calib[1] << 8) | calib[0]);
		dig_t2 = (int16_t)(((int16_t)calib[3] << 8) | calib[2]);
		dig_t3 = (int16_t)(((int16_t)calib[5] << 8) | calib[4]);
		dig_p1 = (uint16_t)(((uint16_t)calib[7] << 8) | calib[6]);
		dig_p2 = (int16_t)(((int16_t)calib[9] << 8) | calib[8]);
		dig_p3 = (int16_t)(((int16_t)calib[11] << 8) | calib[10]);
		dig_p4 = (int16_t)(((int16_t)calib[13] << 8) | calib[12]);
		dig_p5 = (int16_t)(((int16_t)calib[15] << 8) | calib[14]);
		dig_p6 = (int16_t)(((int16_t)calib[17] << 8) | calib[16]);
		dig_p7 = (int16_t)(((int16_t)calib[19] << 8) | calib[18]);
		dig_p8 = (int16_t)(((int16_t)calib[21] << 8) | calib[20]);
		dig_p9 = (int16_t)(((int16_t)calib[23] << 8) | calib[22]);
	}

	int32_t BMP280::readBMP280Temperature()
	{
		uint8_t rawdata[3];  // 20-bit pressure register data stored here
		ReadWriteByte::readBytes(BMP280_ADDRESS, BMP280_TEMP_MSB, 3, &rawdata[0]);
		return (int32_t)(((int32_t)rawdata[0] << 16 | (int32_t)rawdata[1] << 8 | rawdata[2]) >> 4);
	}

	int32_t BMP280::readBMP280Pressure()
	{
		uint8_t rawdata[3];  // 20-bit pressure register data stored here
		ReadWriteByte::readBytes(BMP280_ADDRESS, BMP280_PRESS_MSB, 3, &rawdata[0]);
		return (int32_t)(((int32_t)rawdata[0] << 16 | (int32_t)rawdata[1] << 8 | rawdata[2]) >> 4);
	}

	// returns temperature in degc, resolution is 0.01 degc. output value of
	// 5123 equals 51.23 degc.
	int32_t BMP280::BMP280_compensate_T(int32_t adc_t)
	{
		int32_t var1, var2, t;
		var1 = ((((adc_t >> 3) - ((int32_t)dig_t1 << 1))) * ((int32_t)dig_t2)) >> 11;
		var2 = (((((adc_t >> 4) - ((int32_t)dig_t1)) * ((adc_t >> 4) - ((int32_t)dig_t1))) >> 12) * ((int32_t)dig_t3)) >> 14;
		t_fine = var1 + var2;
		t = (t_fine * 5 + 128) >> 8;
		return t;
	}

	// returns pressure in pa as unsigned 32 bit integer in q24.8 format (24 integer bits and 8
	//fractional bits).
	//output value of 24674867 represents 24674867/256 = 96386.2 pa = 963.862 hpa
	uint32_t BMP280::BMP280_compensate_P(int32_t adc_p)
	{
		long long var1, var2, p;
		var1 = ((long long)t_fine) - 128000;
		var2 = var1 * var1 * (long long)dig_p6;
		var2 = var2 + ((var1*(long long)dig_p5) << 17);
		var2 = var2 + (((long long)dig_p4) << 35);
		var1 = ((var1 * var1 * (long long)dig_p3) >> 8) + ((var1 * (long long)dig_p2) << 12);
		var1 = (((((long long)1) << 47) + var1))*((long long)dig_p1) >> 33;
		if (var1 == 0)
		{
			return 0;
			// avoid exception caused by division by zero
		}
		p = 1048576 - adc_p;
		p = (((p << 31) - var2) * 3125) / var1;
		var1 = (((long long)dig_p9) * (p >> 13) * (p >> 13)) >> 25;
		var2 = (((long long)dig_p8) * p) >> 19;
		p = ((p + var1 + var2) >> 8) + (((long long)dig_p7) << 4);
		return (uint32_t)p;
	}

 void BMP280::newData(int16_t rawPressure, float pressure, int16_t rawTemp, float temp, SENtral sentral){
      rawPressure = sentral.readSENtralData(EM7180_Baro);
      pressure = (float)rawPressure*0.01f + 1013.25f; // pressure in mBar

      // get BMP280 temperature
      rawTemp = sentral.readSENtralData(EM7180_Temp);
      temp = (float)rawTemp*0.01;  // temperature in degrees C
 }

