/*
 * controleurMoteursBrushless.h
 *
 *  Created on: 22 sept. 2022
 *      Author: julien
 */

#ifndef CONTROLEURMOTEURSBRUSHLESS_H_
#define CONTROLEURMOTEURSBRUSHLESS_H_

#include "controleurMoteurs.h"

#define FREQUENCE_PWM       250
#define RESOLUTION_PWM      10
#define CANAL_AVG           0
#define CANAL_AVD           1
#define CANAL_ARG           2
#define CANAL_ARD           3

#define RAPPORT_CYCLIQUE_0  256
#define RAPPORT_CYCLIQUE_100 512

class controleurMoteursBrushless: public controleurMoteurs
{
public:
  controleurMoteursBrushless();
  virtual ~controleurMoteursBrushless();
  controleurMoteursBrushless(const controleurMoteursBrushless &other) = delete;
  controleurMoteursBrushless(controleurMoteursBrushless &&other) = delete;
  controleurMoteursBrushless& operator=(const controleurMoteursBrushless &other) = delete;
  controleurMoteursBrushless& operator=(controleurMoteursBrushless &&other) = delete;

  virtual void ConfigurerMoteurs(uint16_t u16_MoteurAvG, uint16_t u16_MoteurAvD,
      uint16_t u16_MoteurArG, uint16_t u16_MoteurArD) override;

  virtual void FixerNouvellesConsigne(uint16_t u16_ConsigneMoteurAvG,
      uint16_t u16_ConsigneMoteurAvD, uint16_t u16_ConsigneMoteurArG,
      uint16_t u16_ConsigneMoteurArD) override;

};

#endif /* CONTROLEURMOTEURSBRUSHLESS_H_ */
