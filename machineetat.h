/*
 * machineetat.h
 *
 *  Created on: 2 déc. 2022
 *      Author: julien
 */

#ifndef MACHINEETAT_H_
#define MACHINEETAT_H_

#include <stdint.h>

enum class enum_Etats : uint8_t
{
  DEMARRAGE = 0, ATTENTE_ARMEMENT, ARME, ARRET_URGENCE, NBRE_ETATS,
};

class MachineEtat
{
public:
  virtual ~MachineEtat();
  MachineEtat(const MachineEtat &other) = delete;
  MachineEtat(MachineEtat &&other) = delete;
  MachineEtat& operator=(const MachineEtat &other) = delete;
  MachineEtat& operator=(MachineEtat &&other) = delete;

  virtual enum_Etats retourneEtatInterne(void);

  virtual bool transtionEtatversEtat(enum_Etats i_t_EtatActuel, enum_Etats i_t_NouvelEtat);

  virtual uint8_t reglerActionEtat(enum_Etats i_t_Etat, void (*i_pf_Action)(void));

  static MachineEtat* retourneInstance(void);

protected:

  enum_Etats m_t_EtatInterne;

  void (*m_tpf_Action[(uint8_t) enum_Etats::NBRE_ETATS])(void);

  MachineEtat();

};

#endif /* MACHINEETAT_H_ */
