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

  template<typename T>
  T minMax(T valeur, T valeurMini, T valeurMaxi);

  static ControleurPID *m_pt_InstanceControleurPID;

  uint16_t m_u16_Yaw;
  uint16_t m_u16_Pitch;
  uint16_t m_u16_Roll;
  uint16_t m_u16_Throttle;

  uint32_t pulse_length_esc1 = 1000;
  uint32_t pulse_length_esc2 = 1000;
  uint32_t pulse_length_esc3 = 1000;
  uint32_t pulse_length_esc4 = 1000;

  uint16_t *CorrespondanceVoies[e_ListeMouvements_t::NbreMouvements];

  // ------------- Global variables used for PID controller --------------------
  float pid_set_points[3] = { 0, 0, 0 }; // Yaw, Pitch, Roll
  // Errors
  float errors[3];                // Measured errors (compared to instructions) : [Yaw, Pitch, Roll]
  float delta_err[3] = { 0, 0, 0 }; // Error deltas in that order   : Yaw, Pitch, Roll
  float error_sum[3] = { 0, 0, 0 }; // Error sums (used for integral component) : [Yaw, Pitch, Roll]
  float previous_error[3] = { 0, 0, 0 }; // Last errors (used for derivative component) : [Yaw, Pitch, Roll]
  // PID coefficients
  float Kp[3] = { 4.0, 1.3, 1.3 };    // P coefficients in that order : Yaw, Pitch, Roll
  float Ki[3] = { 0.02, 0.04, 0.04 }; // I coefficients in that order : Yaw, Pitch, Roll
  float Kd[3] = { 0, 18, 18 };        // D coefficients in that order : Yaw, Pitch, Roll
  // ---------------------------------------------------------------------------

};

#endif /* CONTROLEURPID_H_ */
