/*
 * calculerangles.h
 *
 *  Created on: 11 oct. 2022
 *      Author: julien
 */

#ifndef CLASSCALCULERANGLES_H_
#define CLASSCALCULERANGLES_H_

#include <stdint.h>
#include "ConstantesPID.h"

class ClassCalculerAngles
{
public:
  ClassCalculerAngles();
  virtual ~ClassCalculerAngles();
  ClassCalculerAngles(const ClassCalculerAngles &other) = delete;
  ClassCalculerAngles(ClassCalculerAngles &&other) = delete;
  ClassCalculerAngles& operator=(const ClassCalculerAngles &other) = delete;
  ClassCalculerAngles& operator=(ClassCalculerAngles &&other) = delete;

  uint8_t Init();

  uint8_t NouvellesValeursMPU6050();
  uint8_t ValeursOffsetGyroMPU6050();

  void NouvellesValeurs(int16_t *i_ps16_AccRAW, int16_t *i_ps16_GyroRAW);
  void ValeursOffsetGyro(int16_t *i_ps16_GyroOffset);

  void ResetAnglesGyro(void);

  void CalculerAngles(void);
  void CalculerAnglesGyro();
  void CalculerAnglesAccelerometre();

  void DonnerDeplacementsAngulaires(float *o_pf_DeplacementsAngulaires);
  void DonnerMesures(float *o_pf_Mesures);

private:
  bool m_b_GyrOInitialise = false;

  int16_t m_ts16_GyroRAW[e_RepereOrthonormal_t::NbreAxes] = { 0, 0, 0 };
  int16_t m_ts16_AccRAW[e_RepereOrthonormal_t::NbreAxes] = { 0, 0, 0 };

  float m_f_GyroAngle[e_RepereOrthonormal_t::NbreAxes] = { 0, 0, 0 };
  float m_tf_AccAngle[e_RepereOrthonormal_t::NbreAxes] = { 0, 0, 0 };

  int16_t m_ts16_GyroOffset[e_RepereOrthonormal_t::NbreAxes] = { 0, 0, 0 };

  /**
   * Real measures on 3 axis calculated from gyro AND accelerometer in that order : Yaw, Pitch, Roll
   *  - Left wing up implies a positive roll
   *  - Nose up implies a positive pitch
   *  - Nose right implies a positive yaw
   */
  float m_tf_Mesures[e_ListeMouvements_t::NbreMouvements] = { 0, 0, 0 };

  // Calculated angular motion on each axis: Yaw, Pitch, Roll
  float m_tf_DeplacementsAngulaires[e_ListeMouvements_t::NbreMouvements] = { 0, 0, 0 };

  static constexpr float m_f_PI = 3.1415927;
  static constexpr float m_f_Frequence = 250.0;
  static constexpr float m_f_SSF_GYRO = 65.5;

};

#endif /* CLASSCALCULERANGLES_H_ */
