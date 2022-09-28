/*
 * ControleurMoteurFactory.h
 *
 *  Created on: 26 sept. 2022
 *      Author: julien
 */

#ifndef CONTROLEURMOTEURSFACTORY_H_
#define CONTROLEURMOTEURSFACTORY_H_

#include "ControleurMoteurs.h"

enum class e_typeMoteur_t
{
  MOTEUR_BRUSHED, MOTEUR_BRUSHLESS,
};

#define FREQUENCE_PWM_BRUSHLESS       250
#define RESOLUTION_PWM_BRUSHLESS      10

#define FREQUENCE_PWM_BRUSHED         5000
#define RESOLUTION_PWM_BRUSHED        10

class ControleurMoteurFactory
{
public:
//  virtual ~ControleurMoteurFactory();
//  ControleurMoteurFactory(const ControleurMoteurFactory &other) = delete;
//  ControleurMoteurFactory(ControleurMoteurFactory &&other) = delete;
//  ControleurMoteurFactory& operator=(const ControleurMoteurFactory &other) = delete;
//  ControleurMoteurFactory& operator=(ControleurMoteurFactory &&other) = delete;

  static ControleurMoteurs* recupererControleurMoteur(e_typeMoteur_t i_e_typeMoteur,
      uint16_t u16_MoteurAvG = 0, uint16_t u16_MoteurAvD = 0, uint16_t u16_MoteurArG = 0,
      uint16_t u16_MoteurArD = 0);

private:
//  ControleurMoteurFactory();

};

#endif /* CONTROLEURMOTEURSFACTORY_H_ */
