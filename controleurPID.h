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
  virtual ~ControleurPID();
  ControleurPID(const ControleurPID &other) = delete;
  ControleurPID(ControleurPID &&other) = delete;
  ControleurPID& operator=(const ControleurPID &other) = delete;
  ControleurPID& operator=(ControleurPID &&other) = delete;

  uint8_t NouvellesValeursCommandes(uint16_t Voie1, uint16_t Voie2, uint16_t Voie3, uint16_t Voie4);

  void NouvellesValeursMouvementsAngulaires(float i_f_MvtYaw, float i_f_MvtPitch,
      float i_f_MvtRoll);

  uint8_t RecupererNouvellesConsignesMoteurs(uint16_t *o_u16_AvG, uint16_t *o_u16_AvD,
      uint16_t *o_u16_ArG, uint16_t *o_u16_ArD);

  static ControleurPID* RecupererInstance(void);
protected:

private:

  ControleurPID();

  void ActualiserControlleurPID();

  void ConfigurerCorrespondanceVoies(e_ListeVoies_t i_e_Yaw = e_ListeVoies_t::Voie4,
      e_ListeVoies_t i_e_Pitch = e_ListeVoies_t::Voie2, e_ListeVoies_t i_e_Roll =
          e_ListeVoies_t::Voie1, e_ListeVoies_t i_e_Throttle = e_ListeVoies_t::Voie3);

  void ResetPID(void);

  void CalculerErreurs(void);

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
  float pid_set_points[e_ListeMouvements_t::NbreMouvements] = { 0, 0, 0 }; // Yaw, Pitch, Roll
  // Errors
  float m_tf_errors[e_ListeMouvements_t::NbreMouvements]; // Measured errors (compared to instructions) : [Yaw, Pitch, Roll]
  float m_tf_delta_err[e_ListeMouvements_t::NbreMouvements] = { 0, 0, 0 }; // Error deltas in that order   : Yaw, Pitch, Roll
  float m_tf_error_sum[e_ListeMouvements_t::NbreMouvements] = { 0, 0, 0 }; // Error sums (used for integral component) : [Yaw, Pitch, Roll]
  float m_tf_previous_error[e_ListeMouvements_t::NbreMouvements] = { 0, 0, 0 }; // Last errors (used for derivative component) : [Yaw, Pitch, Roll]
  // PID coefficients
  float m_tf_Kp[e_ListeMouvements_t::NbreMouvements] = { 4.0, 1.3, 1.3 }; // P coefficients in that order : Yaw, Pitch, Roll
  float m_tf_Ki[e_ListeMouvements_t::NbreMouvements] = { 0.02, 0.04, 0.04 }; // I coefficients in that order : Yaw, Pitch, Roll
  float m_tf_Kd[e_ListeMouvements_t::NbreMouvements] = { 0, 18, 18 }; // D coefficients in that order : Yaw, Pitch, Roll
  // ---------------------------------------------------------------------------

  float angular_motions[e_ListeMouvements_t::NbreMouvements] = { 0, 0, 0 };

  static constexpr float m_f_ValeurMvtMini = -400.0;
  static constexpr float m_f_ValeurMvtMaxi = 400.0;

  static constexpr uint32_t m_u32_impulsionMini = 1100;
  static constexpr uint32_t m_u32_impulsionMaxi = 2000;

};

#endif /* CONTROLEURPID_H_ */
