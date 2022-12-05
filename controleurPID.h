/*
 * controleurPID.h
 *
 *  Created on: 3 oct. 2022
 *      Author: julien
 */

#ifndef CONTROLEURPID_H_
#define CONTROLEURPID_H_

#include <stdint.h>
#include "ConstantesPID.h"

class ControleurPID
{
public:
  ControleurPID();
  virtual ~ControleurPID();
  ControleurPID(const ControleurPID &other) = delete;
  ControleurPID(ControleurPID &&other) = delete;
  ControleurPID& operator=(const ControleurPID &other) = delete;
  ControleurPID& operator=(ControleurPID &&other) = delete;

  uint8_t NouvellesValeursCommandes(uint16_t Voie1, uint16_t Voie2, uint16_t Voie3, uint16_t Voie4);

  void NouvellesValeursMouvementsAngulaires(float i_f_MvtYaw, float i_f_MvtPitch,
      float i_f_MvtRoll);

  void NouvellesValeursMesures(float i_f_MesureYaw, float i_f_MesurePitch, float i_f_MesureRoll);

  uint8_t RecupererNouvellesConsignesMoteurs(uint16_t &o_u16_AvG, uint16_t &o_u16_AvD,
      uint16_t &o_u16_ArG, uint16_t &o_u16_ArD);

protected:

private:

  void ActualiserControlleurPID();

  void ConfigurerCorrespondanceVoies(e_ListeVoies_t i_e_Yaw = e_ListeVoies_t::VoieNum4,
      e_ListeVoies_t i_e_Pitch = e_ListeVoies_t::VoieNum2, e_ListeVoies_t i_e_Roll =
          e_ListeVoies_t::VoieNum1, e_ListeVoies_t i_e_Throttle = e_ListeVoies_t::VoieNum3);

  void ResetPID(void);

  void CalculerErreurs(void);

  void CalculerConsignesYawRollPitch(void);

  float CalculerConsigneYAW(const uint32_t &i_u32_impulsion_Yaw,
      const uint32_t &i_u32_impulsion_Throttle);

  float CalculerConsignerRollOuPitch(float i_f_angle, const uint32_t &i_u32_channel_pulse);

  template<typename T>
  T minMax(T valeur, T valeurMini, T valeurMaxi);

  static ControleurPID *m_pt_InstanceControleurPID;

  uint32_t m_u32_Yaw;
  uint32_t m_u32_Pitch;
  uint32_t m_u32_Roll;
  uint32_t m_u32_Throttle;

  uint32_t m_u32_ImpulsionEsc1 = 1000;
  uint32_t m_u32_ImpulsionEsc2 = 1000;
  uint32_t m_u32_ImpulsionEsc3 = 1000;
  uint32_t m_u32_ImpulsionEsc4 = 1000;

  uint32_t *m_tu32_CorrespondanceVoies[e_ListeMouvements_t::NbreMouvements];

  // ------------- Global variables used for PID controller --------------------
  float m_tf_Consignes_Pid[e_ListeMouvements_t::NbreMouvements] = { 0, 0, 0 }; // Yaw, Pitch, Roll
  // Errors
  float m_tf_Erreurs[e_ListeMouvements_t::NbreMouvements]; // Measured errors (compared to instructions) : [Yaw, Pitch, Roll]
  float m_tf_Erreurs_Diff[e_ListeMouvements_t::NbreMouvements] = { 0, 0, 0 }; // Error deltas in that order   : Yaw, Pitch, Roll
  float m_tf_Erreurs_Integrales[e_ListeMouvements_t::NbreMouvements] = { 0, 0, 0 }; // Error sums (used for integral component) : [Yaw, Pitch, Roll]
  float m_tf_Erreurs_Precedentes[e_ListeMouvements_t::NbreMouvements] = { 0, 0, 0 }; // Last errors (used for derivative component) : [Yaw, Pitch, Roll]
  // PID coefficients
  float m_tf_Kp[e_ListeMouvements_t::NbreMouvements] = { 4.0, 1.3, 1.3 }; // P coefficients in that order : Yaw, Pitch, Roll
  float m_tf_Ki[e_ListeMouvements_t::NbreMouvements] = { 0.02, 0.04, 0.04 }; // I coefficients in that order : Yaw, Pitch, Roll
  float m_tf_Kd[e_ListeMouvements_t::NbreMouvements] = { 0, 18, 18 }; // D coefficients in that order : Yaw, Pitch, Roll
  // ---------------------------------------------------------------------------

  float m_tf_Vitesses_Angulaires[e_ListeMouvements_t::NbreMouvements] = { 0, 0, 0 };

  float m_tf_Mesures_Angulaires[e_ListeMouvements_t::NbreMouvements] = { 0, 0, 0 };

  static constexpr float m_f_ValeurMvtMini = -400.0;
  static constexpr float m_f_ValeurMvtMaxi = 400.0;

  static constexpr uint32_t m_u32_impulsionMini = 1100;
  static constexpr uint32_t m_u32_impulsionMediane = 1500;
  static constexpr uint32_t m_u32_impulsionMaxi = 2000;

  static constexpr uint32_t m_u32_demiTempsMortImpulsion = 8;

};

#endif /* CONTROLEURPID_H_ */
