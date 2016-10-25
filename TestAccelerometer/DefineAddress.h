// DefineAddress.h

#ifndef _DEFINEADDRESS_h
#define _DEFINEADDRESS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define SerialDebug true  // set to true to get Serial output for debugging
//BMP280

#define BMP280_CONFIG     0xF5
#define BMP280_CTRL_MEAS  0xF4
#define BMP280_STATUS     0xF3
#define BMP280_CALIB00    0x88

#define BMP280_TEMP_MSB   0xFA
#define BMP280_PRESS_MSB  0xF7

//EEPROM
#define EM7180_SentralStatus 0x37
#define EM7180_ResetRequest 0x9B

#define MPU9250_ADDR          0x68   // Device address of MPU9250 when ADO = 0
#define ACCEL_XOUT_H     0x3B
#define GYRO_XOUT_H      0x43

#define SMPLRT_DIV      0x19
#define CONFIG           0x1A
#define GYRO_CONFIG      0x1B
#define ACCEL_CONFIG     0x1C
#define ACCEL_CONFIG2    0x1D

#define SELF_TEST_X_ACCEL 0x0D
#define SELF_TEST_Y_ACCEL 0x0E    
#define SELF_TEST_Z_ACCEL 0x0F
#define SELF_TEST_X_GYRO 0x00                  
#define SELF_TEST_Y_GYRO 0x01                                                                          
#define SELF_TEST_Z_GYRO 0x02

#define PWR_MGMT_1       0x6B // Device defaults to the SLEEP mode
#define PWR_MGMT_2       0x6C
#define INT_ENABLE       0x38
#define FIFO_EN          0x23
#define I2C_MST_CTRL     0x24  
#define USER_CTRL        0x6A  // Bit 7 enable DMP, bit 3 reset DMP
#define FIFO_COUNTH      0x72
#define FIFO_R_W         0x74

#define XG_OFFSET_H      0x13  // User-defined trim values for gyroscope
#define XG_OFFSET_L      0x14
#define YG_OFFSET_H      0x15
#define YG_OFFSET_L      0x16
#define ZG_OFFSET_H      0x17
#define ZG_OFFSET_L      0x18
#define XA_OFFSET_H      0x77
#define YA_OFFSET_H      0x7A
#define ZA_OFFSET_H      0x7D


#define AK8963_CNTL      0x0A  // Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0
#define AK8963_ASAX      0x10  // Fuse ROM x-axis sensitivity adjustment value
#define AK8963_ST1       0x02  // data ready status bit 0
#define AK8963_XOUT_L   0x03  // data


//EM7180
#define MPU9250_ADDRESS 0x68 // Device address of MPU9250 when ADO = 0
#define EM7180_ADDRESS 0x28 // Address of the EM7180 SENtral sensor hub
#define EM7180_FeatureFlags       0x39

#define EM7180_ACC_LPF_BW         0x5B  //Register GP36
#define EM7180_GYRO_LPF_BW        0x5C  //Register GP37

#define EM7180_QRateDivisor       0x32  // uint8_t 
#define EM7180_MagRate            0x55
#define EM7180_AccelRate          0x56
#define EM7180_GyroRate           0x57
#define EM7180_BaroRate           0x58

#define EM7180_AlgorithmControl   0x54
#define EM7180_EnableEvents       0x33

#define EM7180_ParamRequest       0x64
#define EM7180_ParamAcknowledge   0x3A
#define EM7180_SavedParamByte0    0x3B
#define EM7180_SavedParamByte1    0x3C
#define EM7180_SavedParamByte2    0x3D
#define EM7180_SavedParamByte3    0x3E

#define EM7180_LoadParamByte0     0x60
#define EM7180_LoadParamByte1     0x61
#define EM7180_LoadParamByte2     0x62
#define EM7180_LoadParamByte3     0x63

#define EM7180_RunStatus          0x92
#define EM7180_AlgorithmStatus    0x38
#define EM7180_PassThruStatus     0x9E   
#define EM7180_EventStatus        0x35
#define EM7180_SensorStatus       0x36

//SENtral
#define EM7180_ROMVersion1        0x70
#define EM7180_ROMVersion2        0x71
#define EM7180_RAMVersion1        0x72
#define EM7180_RAMVersion2        0x73
#define EM7180_ProductID          0x90
#define EM7180_RevisionID         0x91

#define EM7180_HostControl        0x34
#define EM7180_PassThruControl    0xA0

#define EM7180_AX                 0x1A  // int16_t from registers 0x1A-1B
#define EM7180_QX                 0x00  // this is a 32-bit normalized floating point number read from registers 0x00-03
#define EM7180_GX                 0x22  // int16_t from registers 0x22-23
#define EM7180_MX                 0x12  // int16_t from registers 0x12-13

#define EM7180_Baro               0x2A  // start of two-byte MS5637 pressure data, 16-bit signed interger
#define EM7180_Temp               0x2E  // start of two-byte MS5637 temperature data, 16-bit signed interger

//Accelerometer
#define M24512DFM_DATA_ADDRESS   0x50   // Address of the 500 page M24512DRC EEPROM data buffer, 1024 bits (128 8-bit bytes) per page
#define WHO_AM_I_MPU9250 0x75 // Should return 0x71
#define INT_PIN_CFG      0x37
#define AK8963_ADDRESS           0x0C   // Address of magnetometer
#define WHO_AM_I_AK8963  0x00 // should return 0x48
#define BMP280_ADDRESS           0x76   // Address of BMP280 altimeter when ADO = 0
#define BMP280_RESET      0xE0
#define BMP280_ID         0xD0  // should be 0x58
#define EM7180_ErrorRegister      0x50


#endif

