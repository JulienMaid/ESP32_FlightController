/*
 * controleurMoteursBrushless.cpp
 *
 *  Created on: 22 sept. 2022
 *      Author: julien
 */

#include <Arduino.h>
#include "controleurMoteursBrushed.h"

controleurMoteursBrushed::controleurMoteursBrushed()
{
  // TODO Auto-generated constructor stub

}

controleurMoteursBrushed::~controleurMoteursBrushed()
{
  // TODO Auto-generated destructor stub
}

void controleurMoteursBrushed::ConfigurerMoteurs(uint16_t u16_MoteurAvG, uint16_t u16_MoteurAvD,
    uint16_t u16_MoteurArG, uint16_t u16_MoteurArD)
{
  ledcSetup(CANAL_AVG, FREQUENCE_PWM, RESOLUTION_PWM);
  ledcSetup(CANAL_AVD, FREQUENCE_PWM, RESOLUTION_PWM);
  ledcSetup(CANAL_ARG, FREQUENCE_PWM, RESOLUTION_PWM);
  ledcSetup(CANAL_ARD, FREQUENCE_PWM, RESOLUTION_PWM);

  ledcAttachPin(u16_MoteurAvG, CANAL_AVG);
  ledcAttachPin(u16_MoteurAvD, CANAL_AVD);
  ledcAttachPin(u16_MoteurArG, CANAL_ARG);
  ledcAttachPin(u16_MoteurArD, CANAL_ARD);

  ledcWrite(CANAL_AVG, 0);
  ledcWrite(CANAL_AVD, 0);
  ledcWrite(CANAL_ARG, 0);
  ledcWrite(CANAL_ARD, 0);

}

void controleurMoteursBrushed::FixerNouvellesConsigne(uint16_t u16_ConsigneMoteurAvG,
    uint16_t u16_ConsigneMoteurAvD, uint16_t u16_ConsigneMoteurArG, uint16_t u16_ConsigneMoteurArD)
{
  uint16_t u16_RapportCyclique_AvG = 0;
  uint16_t u16_RapportCyclique_AvD = 0;
  uint16_t u16_RapportCyclique_ArG = 0;
  uint16_t u16_RapportCyclique_ArD = 0;

  if (u16_ConsigneMoteurAvG > 100)
  {
    u16_RapportCyclique_AvG = 100;
  }
  else
  {
    u16_RapportCyclique_AvG = u16_ConsigneMoteurAvG;
  }

  if (u16_ConsigneMoteurAvD > 100)
  {
    u16_RapportCyclique_AvD = 100;
  }
  else
  {
    u16_RapportCyclique_AvD = u16_ConsigneMoteurAvD;
  }

  if (u16_ConsigneMoteurArG > 100)
  {
    u16_RapportCyclique_ArG = 100;
  }
  else
  {
    u16_RapportCyclique_ArG = u16_ConsigneMoteurArG;
  }

  if (u16_ConsigneMoteurArD > 100)
  {
    u16_RapportCyclique_ArD = 100;
  }
  else
  {
    u16_RapportCyclique_ArD = u16_ConsigneMoteurArD;
  }

  ledcWrite(CANAL_AVG, u16_RapportCyclique_AvG);
  ledcWrite(CANAL_AVD, u16_RapportCyclique_AvD);
  ledcWrite(CANAL_ARG, u16_RapportCyclique_ArG);
  ledcWrite(CANAL_ARD, u16_RapportCyclique_ArD);

}
