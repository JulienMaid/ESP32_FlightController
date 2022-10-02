/*
 * MPU6050.cpp
 *
 *  Created on: 14 sept. 2022
 *      Author: julien
 */

#include "MPU6050.h"
#include <Arduino.h>
#include <Wire.h>

// ----------------------- MPU variables -------------------------------------

// Average gyro offsets of each axis in that order: X, Y, Z
uint32_t gyro_offset[3] = { 0, 0, 0 };

// Calculated angles from gyro's values in that order: X, Y, Z
float gyro_angle[3] = { 0, 0, 0 };

// Calculated angles from accelerometer's values in that order: X, Y, Z
float acc_angle[3] = { 0, 0, 0 };

// Total 3D acceleration vector in m/s²
uint32_t acc_total_vector;

// Calculated angular motion on each axis: Yaw, Pitch, Roll
float angular_motions[3] = { 0, 0, 0 };

/**
 * Real measures on 3 axis calculated from gyro AND accelerometer in that order : Yaw, Pitch, Roll
 *  - Left wing up implies a positive roll
 *  - Nose up implies a positive pitch
 *  - Nose right implies a positive yaw
 */
float measures[3] = { 0, 0, 0 };

/**
 * Configure gyro and accelerometer precision as following:
 *  - accelerometer: ±8g
 *  - gyro: ±500°/s
 *
 * @see https://www.invensense.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
 */
void setupMpu6050Registers()
{
  // Start I2C communication @ 400kHz
  Wire.begin(21, 22, (uint32_t) 400000);

// Configure power management
  Wire.beginTransmission(MPU_ADDRESS); // Start communication with MPU
  Wire.write(0x6B);                    // Request the PWR_MGMT_1 register
  Wire.write(0x00);                    // Apply the desired configuration to the register
  Wire.endTransmission();              // End the transmission

  // Configure the gyro's sensitivity
  Wire.beginTransmission(MPU_ADDRESS); // Start communication with MPU
  Wire.write(0x1B);                    // Request the GYRO_CONFIG register
  Wire.write(0x08);                    // Apply the desired configuration to the register : ±500°/s
  Wire.endTransmission();              // End the transmission

  // Configure the acceleromter's sensitivity
  Wire.beginTransmission(MPU_ADDRESS); // Start communication with MPU
  Wire.write(0x1C);                    // Request the ACCEL_CONFIG register
  Wire.write(0x10);                    // Apply the desired configuration to the register : ±8g
  Wire.endTransmission();              // End the transmission

  // Configure low pass filter
  Wire.beginTransmission(MPU_ADDRESS); // Start communication with MPU
  Wire.write(0x1A);                    // Request the CONFIG register
  Wire.write(0x03);                    // Set Digital Low Pass Filter about ~43Hz
  Wire.endTransmission();              // End the transmission
}

/**
 * Calibrate MPU6050: take 2000 samples to calculate average offsets.
 * During this step, the quadcopter needs to be static and on a horizontal surface.
 *
 * This function also sends low throttle signal to each ESC to init and prevent them beeping annoyingly.
 *
 * This function might take ~2sec for 2000 samples.
 */
void calibrateMpu6050(uint16_t *o_tu16_gyroOffset)
{
  uint16_t max_samples = 2000;
  // The RAW values got from gyro (in °/sec) in that order: X, Y, Z
  uint16_t tu16_gyroRaw[3];
  // The RAW values got from accelerometer (in m/sec²) in that order: X, Y, Z
  uint16_t tu16_accRaw[3];

  o_tu16_gyroOffset[X] = 0;
  o_tu16_gyroOffset[Y] = 0;
  o_tu16_gyroOffset[Z] = 0;

  for (uint16_t i = 0; i < max_samples; i++)
  {
    readSensor(tu16_accRaw, tu16_gyroRaw);

    o_tu16_gyroOffset[X] += tu16_gyroRaw[X];
    o_tu16_gyroOffset[Y] += tu16_gyroRaw[Y];
    o_tu16_gyroOffset[Z] += tu16_gyroRaw[Z];

    delay(3);
  }

  // Calculate average offsets
  o_tu16_gyroOffset[X] /= max_samples;
  o_tu16_gyroOffset[Y] /= max_samples;
  o_tu16_gyroOffset[Z] /= max_samples;
}

/**
 * Request raw values from MPU6050.
 */
void readSensor(uint16_t *o_tu16_accRaw, uint16_t *o_tu16_gyroRaw)
{
  // MPU's temperature
  uint16_t temperature;

  Wire.beginTransmission(MPU_ADDRESS); // Start communicating with the MPU-6050
  Wire.write(0x3B);                    // Send the requested starting register
  Wire.endTransmission();              // End the transmission
  Wire.requestFrom(MPU_ADDRESS, 14);    // Request 14 bytes from the MPU-6050

  // Wait until all the bytes are received
  while (Wire.available() < 14);

  o_tu16_accRaw[X] = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the acc_raw[X] variable
  o_tu16_accRaw[Y] = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the acc_raw[Y] variable
  o_tu16_accRaw[Z] = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the acc_raw[Z] variable
  temperature = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the temperature variable
  o_tu16_gyroRaw[X] = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the gyro_raw[X] variable
  o_tu16_gyroRaw[Y] = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the gyro_raw[Y] variable
  o_tu16_gyroRaw[Z] = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the gyro_raw[Z] variable
}
