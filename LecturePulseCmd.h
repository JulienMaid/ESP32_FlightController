/*
 * LecturePulseCmd.h
 *
 *  Created on: 29 sept. 2022
 *      Author: julien
 */

#ifndef LECTUREPULSECMD_H_
#define LECTUREPULSECMD_H_

#include <stdint.h>
#include <Arduino.h>

static constexpr uint32_t g_u32_Valeur_Impulsion_Mini = 1000;
static constexpr uint32_t g_u32_Valeur_Impulsion_Maxi = 2000;

enum e_NumeroVoie_t : uint8_t
{
  Voie1 = 0, Voie2, Voie3, Voie4, Voie5, Voie6, Nombre_Voies,
};

enum e_EtatsVoie_t : uint8_t
{
  Inconnu = 0, Haut, Bas,
};

void IRAM_ATTR ISR_ImpulsionCmdVoie1(void);
void IRAM_ATTR ISR_ImpulsionCmdVoie2(void);
void IRAM_ATTR ISR_ImpulsionCmdVoie3(void);
void IRAM_ATTR ISR_ImpulsionCmdVoie4(void);
void IRAM_ATTR ISR_ImpulsionCmdVoie5(void);
void IRAM_ATTR ISR_ImpulsionCmdVoie6(void);

extern volatile uint32_t g_tu32_ImpulsionVoies[e_NumeroVoie_t::Nombre_Voies];

void InitPortCmd(uint8_t i_u8_portVoie1 = 0, uint8_t i_u8_portVoie2 = 0, uint8_t i_u8_portVoie3 = 0,
    uint8_t i_u8_portVoie4 = 0, uint8_t i_u8_portVoie5 = 0, uint8_t i_u8_portVoie6 = 0);

void GestionPortCmd(e_NumeroVoie_t i_e_NumeroVoie);

void LectureImpulsionsCmd(uint32_t *o_tu32_TableauLargeurImpulsion[4]);

#endif /* LECTUREPULSECMD_H_ */
