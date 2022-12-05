/*
 * machineetat.cpp
 *
 *  Created on: 2 déc. 2022
 *      Author: julien
 */

#include "machineetat.h"
#include "trace_debug.h"

MachineEtat* MachineEtat::retourneInstance(void)
{
  static MachineEtat *l_pt_Machine = nullptr;

  if (l_pt_Machine == nullptr)
  {
    l_pt_Machine = new MachineEtat;
  }

  return l_pt_Machine;
}

MachineEtat::MachineEtat() :
    m_t_EtatInterne(enum_Etats::DEMARRAGE)
{
  for (auto l_index = 0; l_index < (uint8_t) enum_Etats::NBRE_ETATS; l_index++)
  {
    m_tpf_Action[l_index] = nullptr;
  }
}

MachineEtat::~MachineEtat()
{
}

enum_Etats MachineEtat::retourneEtatInterne(void)
{
  return m_t_EtatInterne;
}

bool MachineEtat::transtionEtatversEtat(enum_Etats i_t_EtatActuel, enum_Etats i_t_NouvelEtat)
{
  bool l_b_valeurRetournee = false;

  if (m_t_EtatInterne == i_t_EtatActuel)
  {
    m_t_EtatInterne = i_t_NouvelEtat;
    l_b_valeurRetournee = true;
    SEND_VTRACE(INFO, "Transition %d ---> %d", i_t_EtatActuel, i_t_NouvelEtat);

    if (m_tpf_Action[(uint8_t) m_t_EtatInterne] != nullptr)
    {
      m_tpf_Action[(uint8_t) m_t_EtatInterne]();
    }
    else
    {
      SEND_VTRACE(INFO, "Pas d'action de transition associee");
    }

  }
  else
  {
    SEND_VTRACE(ERROR, "Transition %d -x-> %d", i_t_EtatActuel, i_t_NouvelEtat);
  }

  return l_b_valeurRetournee;
}

uint8_t MachineEtat::reglerActionEtat(enum_Etats i_t_Etat, void (*i_pf_Action)(void))
{
  uint8_t l_b_CodeRetour = 1;

  if (i_t_Etat < enum_Etats::NBRE_ETATS)
  {
    if (m_tpf_Action[(uint8_t) i_t_Etat] == nullptr)
    {
      m_tpf_Action[(uint8_t) i_t_Etat] = i_pf_Action;
      l_b_CodeRetour = 0;
    }
  }

  return l_b_CodeRetour;
}
