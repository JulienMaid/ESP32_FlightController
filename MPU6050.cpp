/*
 * MPU6050.cpp
 *
 *  Created on: 14 sept. 2022
 *      Author: julien
 */

#include "MPU6050.h"
#include <Arduino.h>
#include <Wire.h>
#include "ConstantesPID.h"
#include "trace_debug.h"

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
uint8_t calibrateMpu6050(int16_t *o_ts16_gyroOffset)
{
  uint16_t l_u16_max_samples = 2000;
  // The RAW values got from gyro (in °/sec) in that order: X, Y, Z
  int16_t l_ts16_gyroRaw[e_RepereOrthonormal_t::NbreAxes];
  // The RAW values got from accelerometer (in m/sec²) in that order: X, Y, Z
  int16_t l_ts16_accRaw[e_RepereOrthonormal_t::NbreAxes];

  int32_t l_ts32_gyroOffset[e_RepereOrthonormal_t::NbreAxes];

  l_ts32_gyroOffset[e_RepereOrthonormal_t::X] = 0;
  l_ts32_gyroOffset[e_RepereOrthonormal_t::Y] = 0;
  l_ts32_gyroOffset[e_RepereOrthonormal_t::Z] = 0;

  for (uint16_t i = 0; i < l_u16_max_samples; i++)
  {
    if (readSensor(l_ts16_accRaw, l_ts16_gyroRaw) != 0)
    {
      return 1;
    }

    l_ts32_gyroOffset[e_RepereOrthonormal_t::X] += l_ts16_gyroRaw[e_RepereOrthonormal_t::X];
    l_ts32_gyroOffset[e_RepereOrthonormal_t::Y] += l_ts16_gyroRaw[e_RepereOrthonormal_t::Y];
    l_ts32_gyroOffset[e_RepereOrthonormal_t::Z] += l_ts16_gyroRaw[e_RepereOrthonormal_t::Z];

    delay(3);
  }

  // Calculate average offsets
  l_ts32_gyroOffset[e_RepereOrthonormal_t::X] /= l_u16_max_samples;
  l_ts32_gyroOffset[e_RepereOrthonormal_t::Y] /= l_u16_max_samples;
  l_ts32_gyroOffset[e_RepereOrthonormal_t::Z] /= l_u16_max_samples;

  o_ts16_gyroOffset[e_RepereOrthonormal_t::X] = l_ts32_gyroOffset[e_RepereOrthonormal_t::X];
  o_ts16_gyroOffset[e_RepereOrthonormal_t::Y] = l_ts32_gyroOffset[e_RepereOrthonormal_t::Y];
  o_ts16_gyroOffset[e_RepereOrthonormal_t::Z] = l_ts32_gyroOffset[e_RepereOrthonormal_t::Z];

  return 0;
}

/**
 * Request raw values from MPU6050.
 */
uint8_t readSensor(int16_t *o_ts16_accRaw, int16_t *o_ts16_gyroRaw)
{
  // MPU's temperature
  uint16_t temperature;
  uint32_t u32_timeout = 0;

  Wire.beginTransmission(MPU_ADDRESS); // Start communicating with the MPU-6050
  Wire.write(0x3B);                    // Send the requested starting register
  Wire.endTransmission();              // End the transmission
  Wire.requestFrom(MPU_ADDRESS, 14);    // Request 14 bytes from the MPU-6050

  u32_timeout = millis();

  // Wait until all the bytes are received
  while (Wire.available() < 14)
  {
    if ((millis() - u32_timeout) > 1000)
    {
      SEND_VTRACE(ERROR, "Timeout I2C MPU6050");
      return 1;
    }
  }

  o_ts16_accRaw[e_RepereOrthonormal_t::X] = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the acc_raw[X] variable
  o_ts16_accRaw[e_RepereOrthonormal_t::Y] = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the acc_raw[Y] variable
  o_ts16_accRaw[e_RepereOrthonormal_t::Z] = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the acc_raw[Z] variable
  temperature = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the temperature variable
  o_ts16_gyroRaw[e_RepereOrthonormal_t::X] = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the gyro_raw[X] variable
  o_ts16_gyroRaw[e_RepereOrthonormal_t::Y] = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the gyro_raw[Y] variable
  o_ts16_gyroRaw[e_RepereOrthonormal_t::Z] = Wire.read() << 8 | Wire.read(); // Add the low and high byte to the gyro_raw[Z] variable

  return 0;
}
