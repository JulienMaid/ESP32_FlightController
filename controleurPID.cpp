/*
 * controleurPID.cpp
 *
 *  Created on: 3 oct. 2022
 *      Author: julien
 */

#include "controleurPID.h"

ControleurPID::ControleurPID()
{
}

ControleurPID::~ControleurPID()
{
}

uint8_t ControleurPID::NouvellesValeursCommandes(uint16_t Voie1, uint16_t Voie2, uint16_t Voie3,
    uint16_t Voie4)
{
  *CorrespondanceVoies[e_ListeVoies_t::Voie1] = Voie1;
  *CorrespondanceVoies[e_ListeVoies_t::Voie2] = Voie2;
  *CorrespondanceVoies[e_ListeVoies_t::Voie3] = Voie3;
  *CorrespondanceVoies[e_ListeVoies_t::Voie4] = Voie4;

  return 0;
}

uint8_t ControleurPID::RecupererNouvellesConsignesMoteurs(uint16_t *o_u16_AvG, uint16_t *o_u16_AvD,
    uint16_t *o_u16_ArG, uint16_t *o_u16_ArD)
{
  *o_u16_AvG = pulse_length_esc1;
  *o_u16_AvD = pulse_length_esc2;
  *o_u16_ArG = pulse_length_esc3;
  *o_u16_ArG = pulse_length_esc4;

  return 0;
}

void ControleurPID::ResetPID(void)
{
}

ControleurPID* ControleurPID::RecupererInstance(void)
{
  if (m_pt_InstanceControleurPID == nullptr)
  {
    m_pt_InstanceControleurPID = new ControleurPID;

    m_pt_InstanceControleurPID->ConfigurerCorrespondanceVoies();

    m_pt_InstanceControleurPID->ResetPID();

  }

  return m_pt_InstanceControleurPID;
}

void ControleurPID::ActualiserControlleurPID()
{
  /**
   * Calculate motor speed for each motor of an X quadcopter depending on received instructions and measures from sensor
   * by applying PID control.
   *
   * (A) (B)     x
   *   \ /     z ↑
   *    X       \|
   *   / \       +----→ y
   * (C) (D)
   *
   * Motors A & D run clockwise.
   * Motors B & C run counter-clockwise.
   *
   * Each motor output is considered as a servomotor. As a result, value range is about 1000µs to 2000µs
   */
  float yaw_pid = 0;
  float pitch_pid = 0;
  float roll_pid = 0;
  int16_t throttle = m_u16_Throttle; //pulse_length[mode_mapping[THROTTLE]];

  // Initialize motor commands with throttle
  pulse_length_esc1 = throttle;
  pulse_length_esc2 = throttle;
  pulse_length_esc3 = throttle;
  pulse_length_esc4 = throttle;

  // Do not calculate anything if throttle is 0
  if (throttle >= 1012)
  {
    // PID = e.Kp + ∫e.Ki + Δe.Kd
    yaw_pid = (errors[e_ListeMouvements_t::Yaw] * Kp[e_ListeMouvements_t::Yaw])
        + (error_sum[e_ListeMouvements_t::Yaw] * Ki[e_ListeMouvements_t::Yaw])
        + (delta_err[e_ListeMouvements_t::Yaw] * Kd[e_ListeMouvements_t::Yaw]);
    pitch_pid = (errors[e_ListeMouvements_t::Pitch] * Kp[e_ListeMouvements_t::Pitch])
        + (error_sum[e_ListeMouvements_t::Pitch] * Ki[e_ListeMouvements_t::Pitch])
        + (delta_err[e_ListeMouvements_t::Pitch] * Kd[e_ListeMouvements_t::Pitch]);
    roll_pid = (errors[e_ListeMouvements_t::Roll] * Kp[e_ListeMouvements_t::Roll])
        + (error_sum[e_ListeMouvements_t::Roll] * Ki[e_ListeMouvements_t::Roll])
        + (delta_err[e_ListeMouvements_t::Roll] * Kd[e_ListeMouvements_t::Roll]);

    // Keep values within acceptable range. TODO export hard-coded values in variables/const
    yaw_pid = minMax(yaw_pid, (float) -400.0, (float) 400.0);
    pitch_pid = minMax(pitch_pid, (float) -400, (float) 400);
    roll_pid = minMax(roll_pid, (float) -400, (float) 400);

    // Calculate pulse duration for each ESC
    pulse_length_esc1 = throttle - roll_pid - pitch_pid + yaw_pid;
    pulse_length_esc2 = throttle + roll_pid - pitch_pid - yaw_pid;
    pulse_length_esc3 = throttle - roll_pid + pitch_pid - yaw_pid;
    pulse_length_esc4 = throttle + roll_pid + pitch_pid + yaw_pid;
  }

  // Prevent out-of-range-values
  pulse_length_esc1 = minMax(pulse_length_esc1, (uint32_t) 1100, (uint32_t) 2000);
  pulse_length_esc2 = minMax(pulse_length_esc2, (uint32_t) 1100, (uint32_t) 2000);
  pulse_length_esc3 = minMax(pulse_length_esc3, (uint32_t) 1100, (uint32_t) 2000);
  pulse_length_esc4 = minMax(pulse_length_esc4, (uint32_t) 1100, (uint32_t) 2000);

}

void ControleurPID::ConfigurerCorrespondanceVoies(e_ListeVoies_t i_e_Yaw, e_ListeVoies_t i_e_Pitch,
    e_ListeVoies_t i_e_Roll, e_ListeVoies_t i_e_Throttle)
{
  CorrespondanceVoies[i_e_Yaw] = &m_u16_Yaw;
  CorrespondanceVoies[i_e_Pitch] = &m_u16_Pitch;
  CorrespondanceVoies[i_e_Roll] = &m_u16_Roll;
  CorrespondanceVoies[i_e_Throttle] = &m_u16_Throttle;
}

/**
 * Limiter une valeur entre deux bornes
 *
 * @param T valeur     : Valeur à plafonner
 * @param T valeurMini : Valeur minimum
 * @param T valeurMaxi : Valeur maximum
 *
 * @return T
 */

template<typename T>
T ControleurPID::minMax(T valeur, T valeurMini, T valeurMaxi)
{
  if (valeur > valeurMaxi)
  {
    valeur = valeurMaxi;
  }
  else if (valeur < valeurMini)
  {
    valeur = valeurMini;
  }

  return valeur;
}
