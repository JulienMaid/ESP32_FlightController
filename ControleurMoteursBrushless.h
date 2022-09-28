/*
 * controleurMoteursBrushless.h
 *
 *  Created on: 22 sept. 2022
 *      Author: julien
 */

#ifndef CONTROLEURMOTEURSBRUSHLESS_H_
#define CONTROLEURMOTEURSBRUSHLESS_H_

#include "ControleurMoteurs.h"

#define CANAL_AVG           0
#define CANAL_AVD           1
#define CANAL_ARG           2
#define CANAL_ARD           3

#define RAPPORT_CYCLIQUE_0  256
#define RAPPORT_CYCLIQUE_100 512

class ControleurMoteursBrushless: public ControleurMoteurs
{
public:
  ControleurMoteursBrushless();
  virtual ~ControleurMoteursBrushless();
  ControleurMoteursBrushless(const ControleurMoteursBrushless &other) = delete;
  ControleurMoteursBrushless(ControleurMoteursBrushless &&other) = delete;
  ControleurMoteursBrushless& operator=(const ControleurMoteursBrushless &other) = delete;
  ControleurMoteursBrushless& operator=(ControleurMoteursBrushless &&other) = delete;

  virtual void FixerNouvellesConsigne(uint16_t i_u16_ConsigneMoteurAvG,
      uint16_t i_u16_ConsigneMoteurAvD, uint16_t i_u16_ConsigneMoteurArG,
      uint16_t i_u16_ConsigneMoteurArD) override;

  virtual void ConfigurerMoteurs(uint8_t i_u8_MoteurAvG, uint8_t i_u8_MoteurAvD,
      uint8_t i_u8_MoteurArG, uint8_t i_u8_MoteurArD, uint32_t i_u32_frequencePWM,
      uint8_t i_u8_resolutionPWM) override;

protected:

};

#endif /* CONTROLEURMOTEURSBRUSHLESS_H_ */
