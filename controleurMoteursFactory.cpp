/*
 * ControleurMoteurFactory.cpp
 *
 *  Created on: 26 sept. 2022
 *      Author: julien
 */

#include "controleurMoteursFactory.h"
#include "ControleurMoteursBrushed.h"
#include "ControleurMoteursBrushless.h"

ControleurMoteurs* ControleurMoteurFactory::recupererControleurMoteur(e_typeMoteur_t i_e_typeMoteur)
{
  ControleurMoteurs *l_pt_controleurMoteurs = nullptr;

  switch (i_e_typeMoteur)
  {
  case e_typeMoteur_t::MOTEUR_BRUSHED:
  {
    l_pt_controleurMoteurs = new ControleurMoteursBrushed;
    break;
  }
  case e_typeMoteur_t::MOTEUR_BRUSHLESS:
  {
    l_pt_controleurMoteurs = new ControleurMoteursBrushless;
    break;
  }
  default:
  {
    break;
  }
  }

  return l_pt_controleurMoteurs;
}
