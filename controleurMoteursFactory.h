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

class ControleurMoteurFactory
{
public:
//  virtual ~ControleurMoteurFactory();
//  ControleurMoteurFactory(const ControleurMoteurFactory &other) = delete;
//  ControleurMoteurFactory(ControleurMoteurFactory &&other) = delete;
//  ControleurMoteurFactory& operator=(const ControleurMoteurFactory &other) = delete;
//  ControleurMoteurFactory& operator=(ControleurMoteurFactory &&other) = delete;

  static ControleurMoteurs* recupererControleurMoteur(e_typeMoteur_t i_e_typeMoteur);

private:
//  ControleurMoteurFactory();

};

#endif /* CONTROLEURMOTEURSFACTORY_H_ */
