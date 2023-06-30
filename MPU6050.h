/*
 * MPU6050.h
 *
 *  Created on: 14 sept. 2022
 *      Author: julien
 */

#ifndef MPU6050_H_
#define MPU6050_H_

#include <stdint.h>

#define MPU_ADDRESS 0x68  // I2C address of the MPU-6050

void setupMpu6050Registers();

uint8_t calibrateMpu6050(int16_t *o_ts16_gyroOffset);

uint8_t readSensor(int16_t *o_ts16_accRaw, int16_t *o_ts16_gyroRaw);

#endif /* MPU6050_H_ */
