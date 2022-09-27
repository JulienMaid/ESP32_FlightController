/*
 * controleurMoteursBrushless.cpp
 *
 *  Created on: 22 sept. 2022
 *      Author: julien
 */

#include <Arduino.h>
#include "ControleurMoteursBrushed.h"

ControleurMoteursBrushed::ControleurMoteursBrushed()
{
  // TODO Auto-generated constructor stub

}

ControleurMoteursBrushed::~ControleurMoteursBrushed()
{
  // TODO Auto-generated destructor stub
}

void ControleurMoteursBrushed::ConfigurerMoteurs(uint16_t u16_MoteurAvG, uint16_t u16_MoteurAvD,
    uint16_t u16_MoteurArG, uint16_t u16_MoteurArD, uint32_t i_u32_frequencePWM,
    uint8_t i_u8_resolutionPWM)
{
  ledcSetup(CANAL_AVG, i_u32_frequencePWM, i_u8_resolutionPWM);
  ledcSetup(CANAL_AVD, i_u32_frequencePWM, i_u8_resolutionPWM);
  ledcSetup(CANAL_ARG, i_u32_frequencePWM, i_u8_resolutionPWM);
  ledcSetup(CANAL_ARD, i_u32_frequencePWM, i_u8_resolutionPWM);

  ledcAttachPin(u16_MoteurAvG, CANAL_AVG);
  ledcAttachPin(u16_MoteurAvD, CANAL_AVD);
  ledcAttachPin(u16_MoteurArG, CANAL_ARG);
  ledcAttachPin(u16_MoteurArD, CANAL_ARD);

  ledcWrite(CANAL_AVG, 0);
  ledcWrite(CANAL_AVD, 0);
  ledcWrite(CANAL_ARG, 0);
  ledcWrite(CANAL_ARD, 0);

}

void ControleurMoteursBrushed::FixerNouvellesConsigne(uint16_t u16_ConsigneMoteurAvG,
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
