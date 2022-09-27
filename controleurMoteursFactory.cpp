/*
 * ControleurMoteurFactory.cpp
 *
 *  Created on: 26 sept. 2022
 *      Author: julien
 */

#include "controleurMoteursFactory.h"
#include "ControleurMoteursBrushed.h"
#include "ControleurMoteursBrushless.h"

ControleurMoteurs* ControleurMoteurFactory::recupererControleurMoteur(e_typeMoteur_t i_e_typeMoteur,
    uint16_t u16_MoteurAvG, uint16_t u16_MoteurAvD, uint16_t u16_MoteurArG, uint16_t u16_MoteurArD)
{
  ControleurMoteurs *l_pt_controleurMoteurs = nullptr;

  switch (i_e_typeMoteur)
  {
  case e_typeMoteur_t::MOTEUR_BRUSHED:
  {
    l_pt_controleurMoteurs = new ControleurMoteursBrushed;
    l_pt_controleurMoteurs->ConfigurerMoteurs(u16_MoteurAvG, u16_MoteurAvD, u16_MoteurArG,
        u16_MoteurArD, FREQUENCE_PWM_BRUSHED, RESOLUTION_PWM_BRUSHED);

    break;
  }
  case e_typeMoteur_t::MOTEUR_BRUSHLESS:
  {
    l_pt_controleurMoteurs = new ControleurMoteursBrushless;
    l_pt_controleurMoteurs->ConfigurerMoteurs(u16_MoteurAvG, u16_MoteurAvD, u16_MoteurArG,
        u16_MoteurArD, FREQUENCE_PWM_BRUSHLESS, RESOLUTION_PWM_BRUSHLESS);
    break;
  }
  default:
  {
    break;
  }
  }

  return l_pt_controleurMoteurs;
}
