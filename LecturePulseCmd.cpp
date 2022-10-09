#include "LecturePulseCmd.h"
#include "trace_debug.h"

uint32_t tu32_TempsVoies[e_NumeroVoie_t::Nombre_Voies] = { 0 };
volatile uint32_t tu32_ImpulsionVoies[e_NumeroVoie_t::Nombre_Voies] = { 0 };
e_EtatsVoie_t te_EtatVoies[e_NumeroVoie_t::Nombre_Voies];
uint8_t tu8_CorrespondanceNumeroVoies[e_NumeroVoie_t::Nombre_Voies];

void InitPortCmd(uint8_t i_u8_portVoie1, uint8_t i_u8_portVoie2, uint8_t i_u8_portVoie3,
    uint8_t i_u8_portVoie4)
{
  for (auto i = 0; i < e_NumeroVoie_t::Nombre_Voies; i++)
  {
    tu32_TempsVoies[i] = 0;
    tu32_ImpulsionVoies[i] = 0;
    te_EtatVoies[i] = e_EtatsVoie_t::Inconnu;
  }

  tu8_CorrespondanceNumeroVoies[e_NumeroVoie_t::Voie1] = i_u8_portVoie1;
  tu8_CorrespondanceNumeroVoies[e_NumeroVoie_t::Voie2] = i_u8_portVoie2;
  tu8_CorrespondanceNumeroVoies[e_NumeroVoie_t::Voie3] = i_u8_portVoie3;
  tu8_CorrespondanceNumeroVoies[e_NumeroVoie_t::Voie4] = i_u8_portVoie4;

  pinMode(PORT_VOIE1, INPUT_PULLDOWN);
  pinMode(PORT_VOIE2, INPUT_PULLDOWN);
  pinMode(PORT_VOIE3, INPUT_PULLDOWN);
  pinMode(PORT_VOIE4, INPUT_PULLDOWN);
  attachInterrupt(PORT_VOIE1, ISR_ImpulsionCmdVoie1, CHANGE);
  attachInterrupt(PORT_VOIE2, ISR_ImpulsionCmdVoie2, CHANGE);
  attachInterrupt(PORT_VOIE3, ISR_ImpulsionCmdVoie3, CHANGE);
  attachInterrupt(PORT_VOIE4, ISR_ImpulsionCmdVoie4, CHANGE);

}

void IRAM_ATTR ISR_ImpulsionCmdVoie1(void)
{
  GestionPortCmd(e_NumeroVoie_t::Voie1);
}

void IRAM_ATTR ISR_ImpulsionCmdVoie2(void)
{
  GestionPortCmd(e_NumeroVoie_t::Voie2);
}

void IRAM_ATTR ISR_ImpulsionCmdVoie3(void)
{
  GestionPortCmd(e_NumeroVoie_t::Voie3);
}

void IRAM_ATTR ISR_ImpulsionCmdVoie4(void)
{
  GestionPortCmd(e_NumeroVoie_t::Voie4);
}

void GestionPortCmd(e_NumeroVoie_t i_e_NumeroVoie)
{
  uint32_t u32_tempsInit = micros();

  if (digitalRead(tu8_CorrespondanceNumeroVoies[i_e_NumeroVoie]) == HIGH)
  {
    if (te_EtatVoies[i_e_NumeroVoie] == e_EtatsVoie_t::Inconnu)
    {
      te_EtatVoies[i_e_NumeroVoie] = e_EtatsVoie_t::Bas;
    }

    if (te_EtatVoies[i_e_NumeroVoie] == e_EtatsVoie_t::Bas)
    {
      te_EtatVoies[i_e_NumeroVoie] = e_EtatsVoie_t::Haut;
      tu32_TempsVoies[i_e_NumeroVoie] = u32_tempsInit;
    }
  }
  else //LOW
  {
    if (te_EtatVoies[i_e_NumeroVoie] == e_EtatsVoie_t::Haut)
    {
      uint32_t u32_ImpulsionVoie;

      te_EtatVoies[i_e_NumeroVoie] = e_EtatsVoie_t::Bas;
      u32_ImpulsionVoie = u32_tempsInit - tu32_TempsVoies[i_e_NumeroVoie];

      // Bornage de la valeur lue
      if (u32_ImpulsionVoie < VALEUR_IMPULSION_MINI)
      {
        SEND_VTRACE(WARNING, "Warning: u32_ImpulsionVoie < %d", VALEUR_IMPULSION_MINI);
        u32_ImpulsionVoie = VALEUR_IMPULSION_MINI;
      }

      if (u32_ImpulsionVoie > VALEUR_IMPULSION_MAXI)
      {
        SEND_VTRACE(WARNING, "Warning: u32_ImpulsionVoie > %d", VALEUR_IMPULSION_MAXI);
        u32_ImpulsionVoie = VALEUR_IMPULSION_MAXI;
      }

      tu32_ImpulsionVoies[i_e_NumeroVoie] = u32_ImpulsionVoie;
    }
  }
}

void LectureImpulsionsCmd(uint32_t *o_tu32_TableauLargeurImpulsion[4])
{
  uint8_t u8_CR = 0;

  for (auto indexBcle = 0; indexBcle < e_NumeroVoie_t::Nombre_Voies; indexBcle++)
  {
    *o_tu32_TableauLargeurImpulsion[indexBcle] = tu32_ImpulsionVoies[indexBcle];

  }

}
