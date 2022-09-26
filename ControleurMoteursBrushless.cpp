/*
 * controleurMoteursBrushless.cpp
 *
 *  Created on: 22 sept. 2022
 *      Author: julien
 */

#include <Arduino.h>
#include "ControleurMoteursBrushless.h"

ControleurMoteursBrushless::ControleurMoteursBrushless()
{
  // TODO Auto-generated constructor stub

}

ControleurMoteursBrushless::~ControleurMoteursBrushless()
{
  // TODO Auto-generated destructor stub
}

void ControleurMoteursBrushless::ConfigurerMoteurs(uint16_t u16_MoteurAvG, uint16_t u16_MoteurAvD,
    uint16_t u16_MoteurArG, uint16_t u16_MoteurArD)
{
  ledcSetup(CANAL_AVG, FREQUENCE_PWM_BRUSHLESS, RESOLUTION_PWM_BRUSHLESS);
  ledcSetup(CANAL_AVD, FREQUENCE_PWM_BRUSHLESS, RESOLUTION_PWM_BRUSHLESS);
  ledcSetup(CANAL_ARG, FREQUENCE_PWM_BRUSHLESS, RESOLUTION_PWM_BRUSHLESS);
  ledcSetup(CANAL_ARD, FREQUENCE_PWM_BRUSHLESS, RESOLUTION_PWM_BRUSHLESS);

  ledcAttachPin(u16_MoteurAvG, CANAL_AVG);
  ledcAttachPin(u16_MoteurAvD, CANAL_AVD);
  ledcAttachPin(u16_MoteurArG, CANAL_ARG);
  ledcAttachPin(u16_MoteurArD, CANAL_ARD);

  ledcWrite(CANAL_AVG, RAPPORT_CYCLIQUE_0);
  ledcWrite(CANAL_AVD, RAPPORT_CYCLIQUE_0);
  ledcWrite(CANAL_ARG, RAPPORT_CYCLIQUE_0);
  ledcWrite(CANAL_ARD, RAPPORT_CYCLIQUE_0);

}

void ControleurMoteursBrushless::FixerNouvellesConsigne(uint16_t u16_ConsigneMoteurAvG,
    uint16_t u16_ConsigneMoteurAvD, uint16_t u16_ConsigneMoteurArG, uint16_t u16_ConsigneMoteurArD)
{
  uint16_t u16_RapportCyclique_AvG = 0;
  uint16_t u16_RapportCyclique_AvD = 0;
  uint16_t u16_RapportCyclique_ArG = 0;
  uint16_t u16_RapportCyclique_ArD = 0;

  u16_RapportCyclique_AvG = map(u16_ConsigneMoteurAvG, 0, 100, RAPPORT_CYCLIQUE_0,
  RAPPORT_CYCLIQUE_100);
  u16_RapportCyclique_AvD = map(u16_ConsigneMoteurAvD, 0, 100, RAPPORT_CYCLIQUE_0,
  RAPPORT_CYCLIQUE_100);
  u16_RapportCyclique_ArG = map(u16_ConsigneMoteurAvG, 0, 100, RAPPORT_CYCLIQUE_0,
  RAPPORT_CYCLIQUE_100);
  u16_RapportCyclique_ArD = map(u16_ConsigneMoteurAvG, 0, 100, RAPPORT_CYCLIQUE_0,
  RAPPORT_CYCLIQUE_100);

  ledcWrite(CANAL_AVG, u16_RapportCyclique_AvG);
  ledcWrite(CANAL_AVD, u16_RapportCyclique_AvD);
  ledcWrite(CANAL_ARG, u16_RapportCyclique_ArG);
  ledcWrite(CANAL_ARD, u16_RapportCyclique_ArD);

}
