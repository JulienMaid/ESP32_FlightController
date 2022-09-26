/*
 * controleurMoteurs.h
 *
 *  Created on: 22 sept. 2022
 *      Author: julien
 */

#ifndef CONTROLEURMOTEURS_H_
#define CONTROLEURMOTEURS_H_

#include <stdint.h>

class controleurMoteurs
{
public:
  controleurMoteurs();
  virtual ~controleurMoteurs();
  controleurMoteurs(const controleurMoteurs &other) = delete;
  controleurMoteurs& operator=(controleurMoteurs &&other) = delete;
  controleurMoteurs& operator=(const controleurMoteurs &other) = delete;
  controleurMoteurs(controleurMoteurs &&other) = delete;

  virtual void ConfigurerMoteurs(uint16_t u16_MoteurAvG, uint16_t u16_MoteurAvD,
      uint16_t u16_MoteurArG, uint16_t u16_MoteurArD) = 0;

  virtual void FixerNouvellesConsigne(uint16_t u16_ConsigneMoteurAvG,
      uint16_t Consigneu16_MoteurAvD, uint16_t u16_ConsigneMoteurArG,
      uint16_t u16_ConsigneMoteurArD) = 0;

};

#endif /* CONTROLEURMOTEURS_H_ */
