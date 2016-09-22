/*
 * EM7180.h
 *
 *  Created on: Sep 19, 2016
 *      Author: Bianca
 */

#ifndef EM7180_H_
#define EM7180_H_

#include "Byte.h"

#define EM7180_ADDRESS           0x28   // Address of the EM7180 SENtral sensor hub

#define EM7180_ParamAcknowledge   0x3A

#define EM7180_AlgorithmControl   0x54

#define EM7180_LoadParamByte0     0x60
#define EM7180_LoadParamByte1     0x61
#define EM7180_LoadParamByte2     0x62
#define EM7180_LoadParamByte3     0x63
#define EM7180_ParamRequest       0x64

void EM7180_set_gyro_FS(uint16_t gyro_fs);
void EM7180_set_mag_acc_FS(uint16_t mag_fs, uint16_t acc_fs);
void EM7180_set_integer_param(uint8_t param, uint32_t param_val);


#endif /* EM7180_H_ */
