/*
 * controleurMoteurs.h
 *
 *  Created on: 22 sept. 2022
 *      Author: julien
 */

#ifndef CONTROLEURMOTEURS_H_
#define CONTROLEURMOTEURS_H_

#include <stdint.h>

class ControleurMoteurs
{
public:
  ControleurMoteurs();
  virtual ~ControleurMoteurs();
  ControleurMoteurs(const ControleurMoteurs &other) = delete;
  ControleurMoteurs& operator=(ControleurMoteurs &&other) = delete;
  ControleurMoteurs& operator=(const ControleurMoteurs &other) = delete;
  ControleurMoteurs(ControleurMoteurs &&other) = delete;

  virtual void FixerNouvellesConsignePourMille(uint16_t u16_ConsigneMoteurAvG,
      uint16_t Consigneu16_MoteurAvD, uint16_t u16_ConsigneMoteurArG,
      uint16_t u16_ConsigneMoteurArD) = 0;

  virtual void FixerNouvellesConsigneImpulsion(uint16_t u16_ConsigneMoteurAvG,
      uint16_t Consigneu16_MoteurAvD, uint16_t u16_ConsigneMoteurArG,
      uint16_t u16_ConsigneMoteurArD) = 0;

  virtual void ConfigurerMoteurs(uint8_t i_u8_MoteurAvG, uint8_t i_u8_MoteurAvD,
      uint8_t i_u8_MoteurArG, uint8_t i_u8_MoteurArD, uint32_t i_u32_frequencePWM,
      uint8_t i_u8_resolutionPWM) = 0;

protected:

  uint32_t m_u32_FrequencePWM;
  uint8_t m_u8_ResolutionPWM;

};

#endif /* CONTROLEURMOTEURS_H_ */
