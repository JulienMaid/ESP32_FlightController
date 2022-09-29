/*
 * LecturePulseCmd.h
 *
 *  Created on: 29 sept. 2022
 *      Author: julien
 */

#ifndef LECTUREPULSECMD_H_
#define LECTUREPULSECMD_H_

#define PORT_VOIE1  0
#define PORT_VOIE2  0
#define PORT_VOIE3  0
#define PORT_VOIE4  0

#define VALEUR_IMPULSION_MINI 1000
#define VALEUR_IMPULSION_MAXI 2000

enum e_NumeroVoie_t
{
  Voie1 = 0, Voie2, Voie3, Voie4, Nombre_Voies,
};

enum class e_EtatsVoie_t
{
  Inconnu = 0, Haut, Bas,
};

void ISR_ImpulsionCmdVoie1(void);
void ISR_ImpulsionCmdVoie2(void);
void ISR_ImpulsionCmdVoie3(void);
void ISR_ImpulsionCmdVoie4(void);

void GestionPortCmd(e_NumeroVoie_t i_e_NumeroVoie);

void LecturePortCmd(void);

#endif /* LECTUREPULSECMD_H_ */
