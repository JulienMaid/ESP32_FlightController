// @file trace_debug.h
/// <b>Date de creation:</b> 16/12/2015                                                      <br>
///                                                                                          <br>
/// @brief Implementation d'un systeme de trace pour aider au debugage                       <br>
///                                                                                          <br>
///==========================================================================================<br>
/// <b>Donnees SVN:</b>                                                                      <br>
/// $Rev:: 517                                                                              $<br>
/// $Author:: julien                                                                        $<br>
/// $Date:: 2016-07-27 11:21:04 +0200 (mer., 27 juil. 2016)                                  $<br>
///                                                                                          <br>
///==========================================================================================<br>
/// <b>Historique:</b>                                                                       <br>
///                                                                                          <br>
///                                                                                          <br>
///==========================================================================================<br>
/// <b>Description detaillee:</b> (si necessaire)                                            <br>
///                                                                                          <br>
///                                                                                          <br>
///                                                                                          <br>
///==========================================================================================<br>

#ifndef TRACE_DEBUG_H
#define TRACE_DEBUG_H

#define UTILISER_TIMER_SW			0
#define UTILISER_NUMERO_VERSION_H	0

#include <stdint.h>
#include "def_debug_type.h"

#if UTILISER_TIMER_SW == 1
#include "timer_sw.h"
#endif

#include "bank_text_debug.h"

#if UTILISER_NUMERO_VERSION_H == 1
#include "numero_version.h"
#endif

#include <string.h>
#include <stdarg.h>

#if !defined(__FILENAME__)
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

//************************************
//* Declaration de types specifiques *
//*************************************

// Definiton pour stocker les textes correspondants au differents types de traces
typedef struct
{
  type_trace_t Type_Trace;
  const uint8_t *p_Text_Data;
} Txt_Type_Trace_t;

typedef struct
{
  char *p_Text_Data;
  uint32_t Value;
} Bank_Grouped_Values_t;

struct Trace_Methodes
{
  void (*Init)(void);
  uint8_t (*Trace_Simple)(type_trace_t Type_Trace, const char Txt_Donnees[], bool Horodatage);
  uint8_t (*Trace_Num)(type_trace_t Type_Trace, const char Txt_Donnees[], uint32_t Num,
      bool Horodatage);
#if UTILISER_NUMERO_VERSION_H == 1
	uint8_t (* Trace_Version)(type_trace_t Type_Trace, const struct Struct_Version * Data, bool Horodatage);
#endif
  uint8_t (*Trace_Buffer)(type_trace_t Type_Trace, const char Txt_Donnees[], uint8_t *Buffer,
      uint8_t Size, bool Horodatage);
  uint8_t (*Trace_Test_Result)(type_trace_t Type_Trace, const char Txt_Donnees[], uint8_t Num,
      bool Horodatage);
#if UTILISER_TIMER_SW == 1
	uint8_t (* Trace_Timer)(type_trace_t Type_Trace, TimerEvent_t * Timer, type_timer_t Type_Event, bool Horodatage);
#endif
  void (*Set_Max_Debug_Level)(type_trace_t Level);
};

#ifdef TRACE_DEBUG_C
/// @brief Definition des textes des types de trace possibles
Txt_Type_Trace_t Table_Type_Trace[11] =
{
  { NONE, Txt_None},
  { ERROR, Txt_Error},
  { WARNING, Txt_Warning},
  { INFO, Txt_Info},
  { TEST_SECU_RESULTS, Txt_Tst_Secu_Results},
  { DBG1, Txt_Debug1},
  { DBG2, Txt_Debug2},
  { DBG3, Txt_Debug3},
  { DBG4, Txt_Debug4},
  { DBGX, Txt_DebugSpecif},
  { ALL, Txt_All}
};
#else
extern Txt_Type_Trace_t Table_Type_Trace[10];
#endif

//*************************************
//* Prototype des fonctions publiques *
//*************************************

/**
 * @brief Initialise le systeme de trace (RTC + UART)
 * @param void
 * @return void
 */
void Init_Trace_Debug(void);

#define SEND_TRACE(typeTrace, TxtDonnees)  \
    Send_Trace((typeTrace), (TxtDonnees), true, __FILENAME__, __FUNCTION__, __LINE__)

#define SEND_VTRACE(typeTrace, TxtDonnees, ...)  \
    Send_VTrace((typeTrace), true, __FILENAME__, __FUNCTION__, __LINE__, (TxtDonnees), ##__VA_ARGS__)

uint8_t Send_VTrace(type_trace_t Type_Trace, bool Horodatage, const char *i_ps8_nomFichier,
    const char *i_ps8_nomFonction, uint16_t i_u16_numeroLigne, const char *Txt_Donnees, ...);

/**
 * @brief Envoi une trace sur l'UART
 * @param Type_Trace Niveau de trace (pour permettre le filtrage des traces)
 * @param Txt_Donnees Donnees a remonter sur la trace
 * @param Horodatage Demande ou non l'horodatage de la trace
 * @return 0 si OK, 2 si Type de trace trop eleve, 1 si erreurs
 */
uint8_t Send_Trace(type_trace_t Type_Trace, const char Txt_Donnees[], bool Horodatage = true,
    const char *i_ps8_nomFichier = NULL, const char *i_ps8_nomFonction = NULL,
    uint16_t i_u16_numeroLigne = 0);

/**
 * @brief Envoi en trace le contenu d'un buffer (attention taille = 32 octets !!)
 * @param Type_Trace Niveau de trace (pour permettre le filtrage des traces)
 * @param Txt_Donnees Libelle de la donnee a remonter sur la trace
 * @param Buffer Pointeur vers le buffer de donnees a remonter
 * @param Size Taille du buffeur
 * @param Horodatage Demande ou non l'horodatage de la trace
 * @return 0 si OK, 2 si Type de trace trop eleve, 1 si erreurs
 */
uint8_t Send_Trace_Buffer(type_trace_t Type_Trace, const char Txt_Donnees[], uint8_t *Buffer,
    uint8_t Size, bool Horodatage = true);

/**
 * @brief Fixe le niveau maximum de trace a remonter
 * @param Level Niveau maximum de trace a afficher
 */
void Set_Max_Debug_Level(type_trace_t Level);

/**
 * @brief Retourne le niveau maximum de trace a remonter
 */
type_trace_t Get_Max_Debug_Level(void);

/**
 * @brief Test si le niveau de trace est suffisant pour etre ressorti
 * @param Level
 * @return 0 si OK, 1 sinon
 */
uint8_t Test_Trace_Level(type_trace_t Level);

/**
 * @brief Renvoi un pointeur vers le texte correspond au niveau de trace
 * @return pointeur vers le texte correspondant, sinon pointeur NULL
 */
const char* Get_Text_Type_Trace(type_trace_t Type_Trace);

/**
 * @brief Realise la fonction suivante: sprintf(String,"%lu") sans sprintf
 * @param Num Valeur a convertir en chaine de caracteres
 * @param String Pointeur vers le buffer a ecrire
 * @return Renvoi le nombre de digit ecrit dans String
 */
uint8_t Conv_NumToStr(uint32_t Num, char *String);

void ThreadTxTrace(void *Parametre);

#endif /* TRACE_DEBUG_H */
