/*
 * MPU6050.h
 *
 *  Created on: 14 sept. 2022
 *      Author: julien
 */

#ifndef MPU6050_H_
#define MPU6050_H_

#include <stdint.h>

#define X           0     // X axis
#define Y           1     // Y axis
#define Z           2     // Z axis
#define MPU_ADDRESS 0x68  // I2C address of the MPU-6050
#define FREQ        250   // Sampling frequency
#define SSF_GYRO    65.5  // Sensitivity Scale Factor of the gyro from datasheet

#define STOPPED  0
#define STARTING 1
#define STARTED  2

void setupMpu6050Registers();

void calibrateMpu6050(uint16_t *o_tu16_gyroOffset);

void readSensor(uint16_t *o_tu16_accRaw, uint16_t *o_tu16_gyroRaw);

#endif /* MPU6050_H_ */
