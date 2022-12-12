/*
 * controleurPID.cpp
 *
 *  Created on: 3 oct. 2022
 *      Author: julien
 */

#include "controleurPID.h"

ControleurPID::ControleurPID() :
    m_u32_Yaw(m_u32_impulsionMediane), m_u32_Pitch(m_u32_impulsionMediane), m_u32_Roll(
        m_u32_impulsionMediane), m_u32_Throttle(m_u32_impulsionMediane), m_u32_ImpulsionEsc1(
        m_u32_impulsionMini), m_u32_ImpulsionEsc2(m_u32_impulsionMini), m_u32_ImpulsionEsc3(
        m_u32_impulsionMini), m_u32_ImpulsionEsc4(m_u32_impulsionMini)

{
  ConfigurerCorrespondanceVoies();

  ResetPID();
}

ControleurPID::~ControleurPID()
{
}

uint8_t ControleurPID::NouvellesValeursCommandes(uint16_t Voie1, uint16_t Voie2, uint16_t Voie3,
    uint16_t Voie4)
{
  *m_tu32_CorrespondanceVoies[e_ListeVoies_t::VoieNum1] = Voie1;
  *m_tu32_CorrespondanceVoies[e_ListeVoies_t::VoieNum2] = Voie2;
  *m_tu32_CorrespondanceVoies[e_ListeVoies_t::VoieNum3] = Voie3;
  *m_tu32_CorrespondanceVoies[e_ListeVoies_t::VoieNum4] = Voie4;

  return 0;
}

void ControleurPID::NouvellesValeursMouvementsAngulaires(float i_f_MvtYaw, float i_f_MvtPitch,
    float i_f_MvtRoll)
{
  m_tf_Vitesses_Angulaires[e_ListeMouvements_t::Yaw] = i_f_MvtYaw;
  m_tf_Vitesses_Angulaires[e_ListeMouvements_t::Pitch] = i_f_MvtPitch;
  m_tf_Vitesses_Angulaires[e_ListeMouvements_t::Roll] = i_f_MvtRoll;
}

/**
 * Calculeur l'erreur pour le controleur PID
 */
void ControleurPID::CalculerErreurs()
{
  // Calcul de l'erreur courante
  m_tf_Erreurs[e_ListeMouvements_t::Yaw] = m_tf_Vitesses_Angulaires[e_ListeMouvements_t::Yaw]
      - m_tf_Consignes_Pid[e_ListeMouvements_t::Yaw];
  m_tf_Erreurs[e_ListeMouvements_t::Pitch] = m_tf_Vitesses_Angulaires[e_ListeMouvements_t::Pitch]
      - m_tf_Consignes_Pid[e_ListeMouvements_t::Pitch];
  m_tf_Erreurs[e_ListeMouvements_t::Roll] = m_tf_Vitesses_Angulaires[e_ListeMouvements_t::Roll]
      - m_tf_Consignes_Pid[e_ListeMouvements_t::Roll];

  // Calcul de la somme des erreurs: Intregrale
  m_tf_Erreurs_Integrales[e_ListeMouvements_t::Yaw] += m_tf_Erreurs[e_ListeMouvements_t::Yaw];
  m_tf_Erreurs_Integrales[e_ListeMouvements_t::Pitch] += m_tf_Erreurs[e_ListeMouvements_t::Pitch];
  m_tf_Erreurs_Integrales[e_ListeMouvements_t::Roll] += m_tf_Erreurs[e_ListeMouvements_t::Roll];

  // Bornage des valeurs
  m_tf_Erreurs_Integrales[e_ListeMouvements_t::Yaw] = minMax(
      m_tf_Erreurs_Integrales[e_ListeMouvements_t::Yaw],
      m_f_ValeurMvtMini / m_tf_Ki[e_ListeMouvements_t::Yaw],
      m_f_ValeurMvtMaxi / m_tf_Ki[e_ListeMouvements_t::Yaw]);
  m_tf_Erreurs_Integrales[e_ListeMouvements_t::Pitch] = minMax(
      m_tf_Erreurs_Integrales[e_ListeMouvements_t::Pitch],
      m_f_ValeurMvtMini / m_tf_Ki[e_ListeMouvements_t::Pitch],
      m_f_ValeurMvtMaxi / m_tf_Ki[e_ListeMouvements_t::Pitch]);
  m_tf_Erreurs_Integrales[e_ListeMouvements_t::Roll] = minMax(
      m_tf_Erreurs_Integrales[e_ListeMouvements_t::Roll],
      m_f_ValeurMvtMini / m_tf_Ki[e_ListeMouvements_t::Roll],
      m_f_ValeurMvtMaxi / m_tf_Ki[e_ListeMouvements_t::Roll]);

  // Calcul de l'erreur différentielle : Dérivée
  m_tf_Erreurs_Diff[e_ListeMouvements_t::Yaw] = m_tf_Erreurs[e_ListeMouvements_t::Yaw]
      - m_tf_Erreurs_Precedentes[e_ListeMouvements_t::Yaw];
  m_tf_Erreurs_Diff[e_ListeMouvements_t::Pitch] = m_tf_Erreurs[e_ListeMouvements_t::Pitch]
      - m_tf_Erreurs_Precedentes[e_ListeMouvements_t::Pitch];
  m_tf_Erreurs_Diff[e_ListeMouvements_t::Roll] = m_tf_Erreurs[e_ListeMouvements_t::Roll]
      - m_tf_Erreurs_Precedentes[e_ListeMouvements_t::Roll];

  // Sauvegarder l'erreur contrainte pour le prochain cycle
  m_tf_Erreurs_Precedentes[e_ListeMouvements_t::Yaw] = m_tf_Erreurs[e_ListeMouvements_t::Yaw];
  m_tf_Erreurs_Precedentes[e_ListeMouvements_t::Pitch] = m_tf_Erreurs[e_ListeMouvements_t::Pitch];
  m_tf_Erreurs_Precedentes[e_ListeMouvements_t::Roll] = m_tf_Erreurs[e_ListeMouvements_t::Roll];
}

uint8_t ControleurPID::RecupererNouvellesConsignesMoteurs(uint16_t &o_u16_AvG, uint16_t &o_u16_AvD,
    uint16_t &o_u16_ArG, uint16_t &o_u16_ArD)
{
  // 3. Calculate set points of PID controller
  CalculerConsignesYawRollPitch();

  // 4. Calculate errors comparing angular motions to set points
  CalculerErreurs();

  ActualiserControlleurPID();

  o_u16_AvG = m_u32_ImpulsionEsc1;
  o_u16_AvD = m_u32_ImpulsionEsc2;
  o_u16_ArG = m_u32_ImpulsionEsc3;
  o_u16_ArD = m_u32_ImpulsionEsc4;

  return 0;
}

void ControleurPID::ResetPID(void)
{
  m_tf_Erreurs[e_ListeMouvements_t::Yaw] = 0;
  m_tf_Erreurs[e_ListeMouvements_t::Pitch] = 0;
  m_tf_Erreurs[e_ListeMouvements_t::Roll] = 0;

  m_tf_Erreurs_Integrales[e_ListeMouvements_t::Yaw] = 0;
  m_tf_Erreurs_Integrales[e_ListeMouvements_t::Pitch] = 0;
  m_tf_Erreurs_Integrales[e_ListeMouvements_t::Roll] = 0;

  m_tf_Erreurs_Diff[e_ListeMouvements_t::Yaw] = 0;
  m_tf_Erreurs_Diff[e_ListeMouvements_t::Pitch] = 0;
  m_tf_Erreurs_Diff[e_ListeMouvements_t::Roll] = 0;

  m_tf_Erreurs_Precedentes[e_ListeMouvements_t::Yaw] = 0;
  m_tf_Erreurs_Precedentes[e_ListeMouvements_t::Pitch] = 0;
  m_tf_Erreurs_Precedentes[e_ListeMouvements_t::Roll] = 0;
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
  float l_f_YawPid = 0;
  float l_f_PitchPid = 0;
  float l_f_RollPid = 0;

  // Initialize motor commands with throttle
  m_u32_ImpulsionEsc1 = m_u32_Throttle;
  m_u32_ImpulsionEsc2 = m_u32_Throttle;
  m_u32_ImpulsionEsc3 = m_u32_Throttle;
  m_u32_ImpulsionEsc4 = m_u32_Throttle;

  // Do not calculate anything if throttle is 0
  if (m_u32_Throttle >= 1012)
  {
    // PID = e.Kp + ∫e.Ki + Δe.Kd
    l_f_YawPid = (m_tf_Erreurs[e_ListeMouvements_t::Yaw] * m_tf_Kp[e_ListeMouvements_t::Yaw])
        + (m_tf_Erreurs_Integrales[e_ListeMouvements_t::Yaw] * m_tf_Ki[e_ListeMouvements_t::Yaw])
        + (m_tf_Erreurs_Diff[e_ListeMouvements_t::Yaw] * m_tf_Kd[e_ListeMouvements_t::Yaw]);
    l_f_PitchPid =
        (m_tf_Erreurs[e_ListeMouvements_t::Pitch] * m_tf_Kp[e_ListeMouvements_t::Pitch])
            + (m_tf_Erreurs_Integrales[e_ListeMouvements_t::Pitch]
                * m_tf_Ki[e_ListeMouvements_t::Pitch])
            + (m_tf_Erreurs_Diff[e_ListeMouvements_t::Pitch] * m_tf_Kd[e_ListeMouvements_t::Pitch]);
    l_f_RollPid = (m_tf_Erreurs[e_ListeMouvements_t::Roll] * m_tf_Kp[e_ListeMouvements_t::Roll])
        + (m_tf_Erreurs_Integrales[e_ListeMouvements_t::Roll] * m_tf_Ki[e_ListeMouvements_t::Roll])
        + (m_tf_Erreurs_Diff[e_ListeMouvements_t::Roll] * m_tf_Kd[e_ListeMouvements_t::Roll]);

    // Keep values within acceptable range. TODO export hard-coded values in variables/const
    l_f_YawPid = minMax(l_f_YawPid, m_f_ValeurMvtMini, m_f_ValeurMvtMaxi);
    l_f_PitchPid = minMax(l_f_PitchPid, m_f_ValeurMvtMini, m_f_ValeurMvtMaxi);
    l_f_RollPid = minMax(l_f_RollPid, m_f_ValeurMvtMini, m_f_ValeurMvtMaxi);

    // Calculate pulse duration for each ESC
    m_u32_ImpulsionEsc1 = m_u32_Throttle - l_f_RollPid - l_f_PitchPid + l_f_YawPid;
    m_u32_ImpulsionEsc2 = m_u32_Throttle + l_f_RollPid - l_f_PitchPid - l_f_YawPid;
    m_u32_ImpulsionEsc3 = m_u32_Throttle - l_f_RollPid + l_f_PitchPid - l_f_YawPid;
    m_u32_ImpulsionEsc4 = m_u32_Throttle + l_f_RollPid + l_f_PitchPid + l_f_YawPid;
  }

  // Prevent out-of-range-values
  m_u32_ImpulsionEsc1 = minMax(m_u32_ImpulsionEsc1, m_u32_impulsionMini, m_u32_impulsionMaxi);
  m_u32_ImpulsionEsc2 = minMax(m_u32_ImpulsionEsc2, m_u32_impulsionMini, m_u32_impulsionMaxi);
  m_u32_ImpulsionEsc3 = minMax(m_u32_ImpulsionEsc3, m_u32_impulsionMini, m_u32_impulsionMaxi);
  m_u32_ImpulsionEsc4 = minMax(m_u32_ImpulsionEsc4, m_u32_impulsionMini, m_u32_impulsionMaxi);

}

void ControleurPID::ConfigurerCorrespondanceVoies(e_ListeVoies_t i_e_Yaw, e_ListeVoies_t i_e_Pitch,
    e_ListeVoies_t i_e_Roll, e_ListeVoies_t i_e_Throttle)
{
  m_tu32_CorrespondanceVoies[i_e_Yaw] = &m_u32_Yaw;
  m_tu32_CorrespondanceVoies[i_e_Pitch] = &m_u32_Pitch;
  m_tu32_CorrespondanceVoies[i_e_Roll] = &m_u32_Roll;
  m_tu32_CorrespondanceVoies[i_e_Throttle] = &m_u32_Throttle;
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

void ControleurPID::CalculerConsignesYawRollPitch(void)
{
  m_tf_Consignes_Pid[e_ListeMouvements_t::Yaw] = CalculerConsigneYAW(m_u32_Yaw, m_u32_Throttle);
  m_tf_Consignes_Pid[e_ListeMouvements_t::Pitch] = CalculerConsignerRollOuPitch(
      m_tf_Mesures_Angulaires[e_ListeMouvements_t::Pitch], m_u32_Pitch);
  m_tf_Consignes_Pid[e_ListeMouvements_t::Roll] = CalculerConsignerRollOuPitch(
      m_tf_Mesures_Angulaires[e_ListeMouvements_t::Roll], m_u32_Roll);

}

float ControleurPID::CalculerConsigneYAW(const uint32_t &i_u32_impulsion_Yaw,
    const uint32_t &i_u32_impulsion_Throttle)
{
  float l_f_Point_Consigne = 0;

  // Do not yaw when turning off the motors
  if (i_u32_impulsion_Throttle > 1050)
  {
    // There is no notion of angle on this axis as the quadcopter can turn on itself
    l_f_Point_Consigne = CalculerConsignerRollOuPitch(0, i_u32_impulsion_Yaw);
  }

  return l_f_Point_Consigne;

}

float ControleurPID::CalculerConsignerRollOuPitch(float i_f_angle,
    const uint32_t &i_u32_channel_pulse)
{
  float l_f_Ajustement_Niveau = i_f_angle * 15; // Value 15 limits maximum angle value to ±32.8°
  float l_f_Point_Consigne = 0;

  // Need a dead band of 16µs for better result
  if (i_u32_channel_pulse > (m_u32_impulsionMediane + m_u32_demiTempsMortImpulsion))
  {
    l_f_Point_Consigne = i_u32_channel_pulse
        - (m_u32_impulsionMediane + m_u32_demiTempsMortImpulsion);
  }
  else if (i_u32_channel_pulse < (m_u32_impulsionMediane - m_u32_demiTempsMortImpulsion))
  {
    l_f_Point_Consigne = i_u32_channel_pulse
        - (m_u32_impulsionMediane - m_u32_demiTempsMortImpulsion);
  }

  l_f_Point_Consigne -= l_f_Ajustement_Niveau;
  l_f_Point_Consigne /= 3;

  return l_f_Point_Consigne;
}

void ControleurPID::NouvellesValeursMesures(float i_f_MesureYaw, float i_f_MesurePitch,
    float i_f_MesureRoll)
{
  m_tf_Mesures_Angulaires[e_ListeMouvements_t::Yaw] = i_f_MesureYaw;
  m_tf_Mesures_Angulaires[e_ListeMouvements_t::Pitch] = i_f_MesurePitch;
  m_tf_Mesures_Angulaires[e_ListeMouvements_t::Roll] = i_f_MesureRoll;
}
