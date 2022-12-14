/*
 * controleurMoteursBrushless.cpp
 *
 *  Created on: 22 sept. 2022
 *      Author: julien
 */

#include <Arduino.h>
#include "ControleurMoteursBrushed.h"
#include "trace_debug.h"
#include <math.h>

ControleurMoteursBrushed::ControleurMoteursBrushed()
{
  // TODO Auto-generated constructor stub

}

ControleurMoteursBrushed::~ControleurMoteursBrushed()
{
  // TODO Auto-generated destructor stub
}

void ControleurMoteursBrushed::ConfigurerMoteurs(uint8_t i_u8_MoteurAvG, uint8_t i_u8_MoteurAvD,
    uint8_t i_u8_MoteurArG, uint8_t i_u8_MoteurArD, uint32_t i_u32_frequencePWM,
    uint8_t i_u8_resolutionPWM)
{
  m_u32_FrequencePWM = i_u32_frequencePWM;
  m_u8_ResolutionPWM = i_u8_resolutionPWM;

  m_u32_RapportCycliqueMax = pow(2, i_u8_resolutionPWM);

  SEND_VTRACE(INFO, "Moteurs Brushed, F:%d Hz, Res: %d bits, MaxPWM: %d", m_u32_FrequencePWM,
      m_u8_ResolutionPWM, m_u32_RapportCycliqueMax);

  ledcSetup(CANAL_AVG, m_u32_FrequencePWM, m_u8_ResolutionPWM);
  ledcSetup(CANAL_AVD, m_u32_FrequencePWM, m_u8_ResolutionPWM);
  ledcSetup(CANAL_ARG, m_u32_FrequencePWM, m_u8_ResolutionPWM);
  ledcSetup(CANAL_ARD, m_u32_FrequencePWM, m_u8_ResolutionPWM);

  ledcAttachPin(i_u8_MoteurAvG, CANAL_AVG);
  ledcAttachPin(i_u8_MoteurAvD, CANAL_AVD);
  ledcAttachPin(i_u8_MoteurArG, CANAL_ARG);
  ledcAttachPin(i_u8_MoteurArD, CANAL_ARD);

  ledcWrite(CANAL_AVG, 0);
  ledcWrite(CANAL_AVD, 0);
  ledcWrite(CANAL_ARG, 0);
  ledcWrite(CANAL_ARD, 0);

}

void ControleurMoteursBrushed::FixerNouvellesConsignePourMille(uint16_t i_u16_ConsigneMoteurAvG,
    uint16_t i_u16_ConsigneMoteurAvD, uint16_t i_u16_ConsigneMoteurArG,
    uint16_t i_u16_ConsigneMoteurArD)
{
  uint16_t l_u16_RapportCyclique_AvG = 0;
  uint16_t l_u16_RapportCyclique_AvD = 0;
  uint16_t l_u16_RapportCyclique_ArG = 0;
  uint16_t l_u16_RapportCyclique_ArD = 0;

  SEND_VTRACE(INFO, "AvG: %d, AvD: %d; ArG: %d, ArD: %d", i_u16_ConsigneMoteurAvG,
      i_u16_ConsigneMoteurAvD, i_u16_ConsigneMoteurArG, i_u16_ConsigneMoteurArD);

  l_u16_RapportCyclique_AvG = map(i_u16_ConsigneMoteurAvG, 0, 1000, 0, m_u32_RapportCycliqueMax);
  l_u16_RapportCyclique_AvD = map(i_u16_ConsigneMoteurAvD, 0, 1000, 0, m_u32_RapportCycliqueMax);
  l_u16_RapportCyclique_ArG = map(i_u16_ConsigneMoteurArG, 0, 1000, 0, m_u32_RapportCycliqueMax);
  l_u16_RapportCyclique_ArD = map(i_u16_ConsigneMoteurArD, 0, 1000, 0, m_u32_RapportCycliqueMax);

  ledcWrite(CANAL_AVG, l_u16_RapportCyclique_AvG);
  ledcWrite(CANAL_AVD, l_u16_RapportCyclique_AvD);
  ledcWrite(CANAL_ARG, l_u16_RapportCyclique_ArG);
  ledcWrite(CANAL_ARD, l_u16_RapportCyclique_ArD);
}

void ControleurMoteursBrushed::FixerNouvellesConsigneImpulsion(uint16_t i_u16_ConsigneMoteurAvG,
    uint16_t i_u16_ConsigneMoteurAvD, uint16_t i_u16_ConsigneMoteurArG,
    uint16_t i_u16_ConsigneMoteurArD)
{
  uint16_t l_u16_RapportCyclique_AvG = 0;
  uint16_t l_u16_RapportCyclique_AvD = 0;
  uint16_t l_u16_RapportCyclique_ArG = 0;
  uint16_t l_u16_RapportCyclique_ArD = 0;

  SEND_VTRACE(DBG2, "AvG: %d, AvD: %d; ArG: %d, ArD: %d", i_u16_ConsigneMoteurAvG,
      i_u16_ConsigneMoteurAvD, i_u16_ConsigneMoteurArG, i_u16_ConsigneMoteurArD);

  l_u16_RapportCyclique_AvG = map(i_u16_ConsigneMoteurAvG, 1000, 2000, 0, m_u32_RapportCycliqueMax);
  l_u16_RapportCyclique_AvD = map(i_u16_ConsigneMoteurAvD, 1000, 2000, 0, m_u32_RapportCycliqueMax);
  l_u16_RapportCyclique_ArG = map(i_u16_ConsigneMoteurArG, 1000, 2000, 0, m_u32_RapportCycliqueMax);
  l_u16_RapportCyclique_ArD = map(i_u16_ConsigneMoteurArD, 1000, 2000, 0, m_u32_RapportCycliqueMax);

  SEND_VTRACE(DBG2, "AvG: %d, AvD: %d; ArG: %d, ArD: %d", l_u16_RapportCyclique_AvG,
      l_u16_RapportCyclique_AvD, l_u16_RapportCyclique_ArG, l_u16_RapportCyclique_ArD);

  ledcWrite(CANAL_AVG, l_u16_RapportCyclique_AvG);
  ledcWrite(CANAL_AVD, l_u16_RapportCyclique_AvD);
  ledcWrite(CANAL_ARG, l_u16_RapportCyclique_ArG);
  ledcWrite(CANAL_ARD, l_u16_RapportCyclique_ArD);

}
