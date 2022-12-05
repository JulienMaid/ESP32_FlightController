/*
 * actionmachineetat.cpp
 *
 *  Created on: 5 déc. 2022
 *      Author: julien
 */

#include "actionmachineetat.h"

#include "OTAUpdate.h"
#include "controleurMoteursFactory.h"

extern ControleurMoteurs *g_pt_ControleurMoteurs;

void ActionArmement(void)
{
  StopperServeurOTA();
}

void ActionDesarmement(void)
{
  DemarrerServeurOTA("");
}

void ActionArretUrgence(void)
{
  if (g_pt_ControleurMoteurs != nullptr)
  {
    g_pt_ControleurMoteurs->FixerNouvellesConsignePourMille(0, 0, 0, 0);
  }
}
