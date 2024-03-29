/*
 * dalculerangles.cpp
 *
 *  Created on: 11 oct. 2022
 *      Author: julien
 */

#include "calculerangles.h"

#include <string.h>
#include <math.h>
#include "MPU6050.h"
#include "trace_debug.h"

ClassCalculerAngles::ClassCalculerAngles()
{
  m_pt_ReAssignationAxes = new AssignationAxes(m_ti16_CorrectionAxeX, m_ti16_CorrectionAxeY,
      m_ti16_CorrectionAxeZ);
}

uint8_t ClassCalculerAngles::Init()
{
  SEND_VTRACE(INFO, "Configuration MPU6050");
  setupMpu6050Registers();

  SEND_VTRACE(INFO, "Calibration MPU6050...");
  if (ValeursOffsetGyroMPU6050() == 0)
  {
    SEND_VTRACE(INFO, "Calibration MPU6050 Faite");
    SEND_VTRACE(INFO, "Offset Gyro: X:%d Y:%d Z:%d", m_ts16_GyroOffset[e_RepereOrthonormal_t::X],
        m_ts16_GyroOffset[e_RepereOrthonormal_t::Y], m_ts16_GyroOffset[e_RepereOrthonormal_t::Z]);
  }
  else
  {
    SEND_VTRACE(ERROR, "Erreur Calibration MPU6050");
    return 1;
  }

  return 0;
}

ClassCalculerAngles::~ClassCalculerAngles()
{
  if (m_pt_ReAssignationAxes != nullptr)
  {
    delete m_pt_ReAssignationAxes;
  }
}

void ClassCalculerAngles::NouvellesValeurs(int16_t *i_ps16_AccRAW, int16_t *i_ps16_GyroRAW)
{
  memcpy(m_ts16_AccRAW, i_ps16_AccRAW, sizeof(m_ts16_AccRAW));
  memcpy(m_ts16_GyroRAW, i_ps16_GyroRAW, sizeof(m_ts16_GyroRAW));
}

/**
 * Reset gyro's angles with accelerometer's angles.
 */
void ClassCalculerAngles::ResetAnglesGyro()
{
  m_f_GyroAngle[e_RepereOrthonormal_t::X] = m_tf_AccAngle[e_RepereOrthonormal_t::X];
  m_f_GyroAngle[e_RepereOrthonormal_t::Y] = m_tf_AccAngle[e_RepereOrthonormal_t::Y];
}

void ClassCalculerAngles::CalculerAngles()
{
  CalculerAnglesGyro();
  CalculerAnglesAccelerometre();

  if (m_b_GyrOInitialise)
  {
    // Correct the drift of the gyro with the accelerometer
    m_f_GyroAngle[e_RepereOrthonormal_t::X] = m_f_GyroAngle[e_RepereOrthonormal_t::X] * 0.9996
        + m_tf_AccAngle[e_RepereOrthonormal_t::X] * 0.0004;
    m_f_GyroAngle[e_RepereOrthonormal_t::Y] = m_f_GyroAngle[e_RepereOrthonormal_t::Y] * 0.9996
        + m_tf_AccAngle[e_RepereOrthonormal_t::Y] * 0.0004;
  }
  else
  {
    // At very first start, init gyro angles with accelerometer angles
    ResetAnglesGyro();

    m_b_GyrOInitialise = true;
  }

  // To dampen the pitch and roll angles a complementary filter is used
  m_tf_Mesures[e_ListeMouvements_t::Roll] = m_tf_Mesures[e_ListeMouvements_t::Roll] * 0.9
      + m_f_GyroAngle[e_RepereOrthonormal_t::X] * 0.1;
  m_tf_Mesures[e_ListeMouvements_t::Pitch] = m_tf_Mesures[e_ListeMouvements_t::Pitch] * 0.9
      + m_f_GyroAngle[e_RepereOrthonormal_t::Y] * 0.1;
  m_tf_Mesures[e_ListeMouvements_t::Yaw] = -m_ts16_GyroRAW[e_RepereOrthonormal_t::Z] / m_f_SSF_GYRO; // Store the angular motion for this axis

  // Apply low-pass filter (10Hz cutoff frequency)
  m_tf_DeplacementsAngulaires[e_ListeMouvements_t::Roll] = 0.7
      * m_tf_DeplacementsAngulaires[e_ListeMouvements_t::Roll]
      + 0.3 * m_ts16_GyroRAW[e_RepereOrthonormal_t::X] / m_f_SSF_GYRO;
  m_tf_DeplacementsAngulaires[e_ListeMouvements_t::Pitch] = 0.7
      * m_tf_DeplacementsAngulaires[e_ListeMouvements_t::Pitch]
      + 0.3 * m_ts16_GyroRAW[e_RepereOrthonormal_t::Y] / m_f_SSF_GYRO;
  m_tf_DeplacementsAngulaires[e_ListeMouvements_t::Yaw] = 0.7
      * m_tf_DeplacementsAngulaires[e_ListeMouvements_t::Yaw]
      + 0.3 * m_ts16_GyroRAW[e_RepereOrthonormal_t::Z] / m_f_SSF_GYRO;
}

void ClassCalculerAngles::CalculerAnglesGyro()
{
// Subtract offsets
  m_ts16_GyroRAW[e_RepereOrthonormal_t::X] -= m_ts16_GyroOffset[e_RepereOrthonormal_t::X];
  m_ts16_GyroRAW[e_RepereOrthonormal_t::Y] -= m_ts16_GyroOffset[e_RepereOrthonormal_t::Y];
  m_ts16_GyroRAW[e_RepereOrthonormal_t::Z] -= m_ts16_GyroOffset[e_RepereOrthonormal_t::Z];

// Angle calculation using integration
  m_f_GyroAngle[e_RepereOrthonormal_t::X] += (m_ts16_GyroRAW[e_RepereOrthonormal_t::X]
      / (m_f_Frequence * m_f_SSF_GYRO));
  m_f_GyroAngle[e_RepereOrthonormal_t::Y] += (-m_ts16_GyroRAW[e_RepereOrthonormal_t::Y]
      / (m_f_Frequence * m_f_SSF_GYRO)); // Change sign to match the accelerometer's one

// Transfer roll to pitch if IMU has yawed
  m_f_GyroAngle[e_RepereOrthonormal_t::Y] += m_f_GyroAngle[e_RepereOrthonormal_t::X]
      * sin(
          m_ts16_GyroRAW[e_RepereOrthonormal_t::Z]
              * (m_f_PI / (m_f_Frequence * m_f_SSF_GYRO * 180)));
  m_f_GyroAngle[e_RepereOrthonormal_t::X] -= m_f_GyroAngle[e_RepereOrthonormal_t::Y]
      * sin(
          m_ts16_GyroRAW[e_RepereOrthonormal_t::Z]
              * (m_f_PI / (m_f_Frequence * m_f_SSF_GYRO * 180)));
}

void ClassCalculerAngles::CalculerAnglesAccelerometre()
{
// Total 3D acceleration vector in m/s²
  int32_t l_s32_ValeurAccelerationTotale;

// Calculate total 3D acceleration vector : √(X² + Y² + Z²)
  l_s32_ValeurAccelerationTotale = sqrt(
      pow(m_ts16_AccRAW[e_RepereOrthonormal_t::X], 2)
          + pow(m_ts16_AccRAW[e_RepereOrthonormal_t::Y], 2)
          + pow(m_ts16_AccRAW[e_RepereOrthonormal_t::Z], 2));

// To prevent asin to produce a NaN, make sure the input value is within [-1;+1]
  if (abs(m_ts16_AccRAW[e_RepereOrthonormal_t::X]) < l_s32_ValeurAccelerationTotale)
  {
    m_tf_AccAngle[e_RepereOrthonormal_t::X] = asin(
        (float) m_ts16_AccRAW[e_RepereOrthonormal_t::Y] / l_s32_ValeurAccelerationTotale)
        * (180 / m_f_PI); // asin gives angle in radian. Convert to degree multiplying by 180/pi
  }

  if (abs(m_ts16_AccRAW[e_RepereOrthonormal_t::Y]) < l_s32_ValeurAccelerationTotale)
  {
    m_tf_AccAngle[e_RepereOrthonormal_t::Y] = asin(
        (float) m_ts16_AccRAW[e_RepereOrthonormal_t::X] / l_s32_ValeurAccelerationTotale)
        * (180 / m_f_PI);
  }

}

void ClassCalculerAngles::ValeursOffsetGyro(int16_t *i_ps16_GyroOffset)
{
  memcpy(m_ts16_GyroOffset, i_ps16_GyroOffset, sizeof(m_ts16_GyroOffset));
}

void ClassCalculerAngles::DonnerDeplacementsAngulaires(float *o_pf_DeplacementsAngulaires)
{
  memcpy(o_pf_DeplacementsAngulaires, m_tf_DeplacementsAngulaires,
      sizeof(m_tf_DeplacementsAngulaires));
}

void ClassCalculerAngles::DonnerMesures(float *o_pf_Mesures)
{
  memcpy(o_pf_Mesures, m_tf_Mesures, sizeof(m_tf_Mesures));
}

uint8_t ClassCalculerAngles::NouvellesValeursMPU6050()
{
  uint8_t l_u8_cr = 0xff;
  int16_t l_ts16_AccRAW[3] = { 0 };
  int16_t l_ts16_GyroRAW[3] = { 0 };

  l_u8_cr = readSensor(l_ts16_AccRAW, l_ts16_GyroRAW);

  if (l_u8_cr == 0)
  {
    m_pt_ReAssignationAxes->ReAssignation(l_ts16_AccRAW, m_ts16_AccRAW);
    m_pt_ReAssignationAxes->ReAssignation(l_ts16_GyroRAW, m_ts16_GyroRAW);
  }

  return l_u8_cr;
}

uint8_t ClassCalculerAngles::ValeursOffsetGyroMPU6050()
{
  uint8_t l_u8_cr = 0xff;
  int16_t l_ts16_GyroOffset[3] = { 0 };

  l_u8_cr = calibrateMpu6050(l_ts16_GyroOffset);

  if (l_u8_cr == 0)
  {
    m_pt_ReAssignationAxes->ReAssignation(l_ts16_GyroOffset, m_ts16_GyroOffset);
  }

  return l_u8_cr;
}
