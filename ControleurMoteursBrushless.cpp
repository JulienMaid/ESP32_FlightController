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

void ControleurMoteursBrushless::ConfigurerMoteurs(uint8_t i_u8_MoteurAvG, uint8_t i_u8_MoteurAvD,
    uint8_t i_u8_MoteurArG, uint8_t i_u8_MoteurArD, uint32_t i_u32_frequencePWM,
    uint8_t i_u8_resolutionPWM)
{
  m_u32_FrequencePWM = i_u32_frequencePWM;
  m_u8_ResolutionPWM = i_u8_resolutionPWM;

  ledcSetup(CANAL_AVG, m_u32_FrequencePWM, m_u8_ResolutionPWM);
  ledcSetup(CANAL_AVD, m_u32_FrequencePWM, m_u8_ResolutionPWM);
  ledcSetup(CANAL_ARG, m_u32_FrequencePWM, m_u8_ResolutionPWM);
  ledcSetup(CANAL_ARD, m_u32_FrequencePWM, m_u8_ResolutionPWM);

  ledcAttachPin(i_u8_MoteurAvG, CANAL_AVG);
  ledcAttachPin(i_u8_MoteurAvD, CANAL_AVD);
  ledcAttachPin(i_u8_MoteurArG, CANAL_ARG);
  ledcAttachPin(i_u8_MoteurArD, CANAL_ARD);

  ledcWrite(CANAL_AVG, RAPPORT_CYCLIQUE_0);
  ledcWrite(CANAL_AVD, RAPPORT_CYCLIQUE_0);
  ledcWrite(CANAL_ARG, RAPPORT_CYCLIQUE_0);
  ledcWrite(CANAL_ARD, RAPPORT_CYCLIQUE_0);

}

void ControleurMoteursBrushless::FixerNouvellesConsigne(uint16_t i_u16_ConsigneMoteurAvG,
    uint16_t i_u16_ConsigneMoteurAvD, uint16_t i_u16_ConsigneMoteurArG,
    uint16_t i_u16_ConsigneMoteurArD)
{
  uint16_t u16_RapportCyclique_AvG = 0;
  uint16_t u16_RapportCyclique_AvD = 0;
  uint16_t u16_RapportCyclique_ArG = 0;
  uint16_t u16_RapportCyclique_ArD = 0;

  u16_RapportCyclique_AvG = map(i_u16_ConsigneMoteurAvG, 0, 100, RAPPORT_CYCLIQUE_0,
  RAPPORT_CYCLIQUE_100);
  u16_RapportCyclique_AvD = map(i_u16_ConsigneMoteurAvD, 0, 100, RAPPORT_CYCLIQUE_0,
  RAPPORT_CYCLIQUE_100);
  u16_RapportCyclique_ArG = map(i_u16_ConsigneMoteurArG, 0, 100, RAPPORT_CYCLIQUE_0,
  RAPPORT_CYCLIQUE_100);
  u16_RapportCyclique_ArD = map(i_u16_ConsigneMoteurArD, 0, 100, RAPPORT_CYCLIQUE_0,
  RAPPORT_CYCLIQUE_100);

  ledcWrite(CANAL_AVG, u16_RapportCyclique_AvG);
  ledcWrite(CANAL_AVD, u16_RapportCyclique_AvD);
  ledcWrite(CANAL_ARG, u16_RapportCyclique_ArG);
  ledcWrite(CANAL_ARD, u16_RapportCyclique_ArD);

}
