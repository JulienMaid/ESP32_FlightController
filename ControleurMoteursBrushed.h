/*
 * controleurMoteursBrushless.h
 *
 *  Created on: 22 sept. 2022
 *      Author: julien
 */

#ifndef CONTROLEURMOTEURSBRUSHED_H_
#define CONTROLEURMOTEURSBRUSHED_H_

#include "ControleurMoteurs.h"

#define FREQUENCE_PWM       5000
#define RESOLUTION_PWM      10
#define CANAL_AVG           0
#define CANAL_AVD           1
#define CANAL_ARG           2
#define CANAL_ARD           3

class ControleurMoteursBrushed: public ControleurMoteurs
{
public:
  ControleurMoteursBrushed();
  virtual ~ControleurMoteursBrushed();
  ControleurMoteursBrushed(const ControleurMoteursBrushed &other) = delete;
  ControleurMoteursBrushed(ControleurMoteursBrushed &&other) = delete;
  ControleurMoteursBrushed& operator=(const ControleurMoteursBrushed &other) = delete;
  ControleurMoteursBrushed& operator=(ControleurMoteursBrushed &&other) = delete;

  virtual void ConfigurerMoteurs(uint16_t u16_MoteurAvG, uint16_t u16_MoteurAvD,
      uint16_t u16_MoteurArG, uint16_t u16_MoteurArD) override;

  virtual void FixerNouvellesConsigne(uint16_t u16_ConsigneMoteurAvG,
      uint16_t u16_ConsigneMoteurAvD, uint16_t u16_ConsigneMoteurArG,
      uint16_t u16_ConsigneMoteurArD) override;

};

#endif /* CONTROLEURMOTEURSBRUSHED_H_ */
