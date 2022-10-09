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

#define PORT_VOIE1  0
#define PORT_VOIE2  0
#define PORT_VOIE3  0
#define PORT_VOIE4  0

#define VALEUR_IMPULSION_MINI (uint32_t)1000
#define VALEUR_IMPULSION_MAXI (uint32_t)2000

enum e_NumeroVoie_t : uint8_t
{
  Voie1 = 0, Voie2, Voie3, Voie4, Nombre_Voies,
};

enum e_EtatsVoie_t : uint8_t
{
  Inconnu = 0, Haut, Bas,
};

void IRAM_ATTR ISR_ImpulsionCmdVoie1(void);
void IRAM_ATTR ISR_ImpulsionCmdVoie2(void);
void IRAM_ATTR ISR_ImpulsionCmdVoie3(void);
void IRAM_ATTR ISR_ImpulsionCmdVoie4(void);

void InitPortCmd(uint8_t i_u8_portVoie1 = 0, uint8_t i_u8_portVoie2 = 0, uint8_t i_u8_portVoie3 = 0,
    uint8_t i_u8_portVoie4 = 0);

void GestionPortCmd(e_NumeroVoie_t i_e_NumeroVoie);

void LectureImpulsionsCmd(uint32_t *o_tu32_TableauLargeurImpulsion[4]);

#endif /* LECTUREPULSECMD_H_ */
