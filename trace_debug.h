// @file trace_deb.h
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

#include <stdint.h>
#include "def_debug_type.h"

#include <string.h>
#include <stdarg.h>
#include <sstream>

#if !defined(__FILENAME__)
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

//************************************
//* Declaration de types specifiques *
//*************************************

// Definiton pour stocker les textes correspondants au differents types de traces
typedef struct
{
  e_type_trace_t Type_Trace;
  const uint8_t *p_Text_Data;
} Txt_Type_Trace_t;

/// @brief Definition des textes des types de trace possibles
extern Txt_Type_Trace_t Table_Type_Trace[11];

//*************************************
//* Prototype des fonctions publiques *
//*************************************

/**
 * @brief Initialise le systeme de trace (RTC + UART)
 * @param void
 * @return void
 */
void Init_Trace_Debug(bool i_b_TraceSerie = true, bool i_b_TraceUDP = true,
    std::string i_t_IPTraceUDP = "", uint16_t i_u16_PortDestTracesUDP = 1234);

#define SEND_TRACE(typeTrace, TxtDonnees)  \
    Send_Trace((typeTrace), (TxtDonnees), true, __FILENAME__, __FUNCTION__, __LINE__)

#define SEND_VTRACE(typeTrace, TxtDonnees, ...)  \
    Send_VTrace((typeTrace), true, __FILENAME__, __FUNCTION__, __LINE__, (TxtDonnees), ##__VA_ARGS__)

/**
 * @brief Envoi d'un trace avec arguments variables facon "Printf"
 * @param Type_Trace Niveau de trace (pour permettre le filtrage des traces)
 * @param Horodatage Demande ou non l'horodatage de la trace
 * @param i_ps8_nomFichier Nom du ficher d'où est appelle la fonction de trace
 * @param i_ps8_nomFonction Nom de la fonction d'où est appelle la fonction de trace
 * @param i_u16_numeroLigne Numero de la ligne d'où est appelle la fonction de trace
 * @param Buffer Pointeur vers le buffer de donnees a remonter
 * @param Txt_Donnees Libelle de la donnee a remonter sur la trace
 * @return 0 si OK, 2 si Type de trace trop eleve, 1 si erreurs
 *
 * Cette fonction peut être appellee seule, mais preferer l'appel à travers SEND_VTRACE
 * qui prendra en charge de renseigner les champs nomFicher, nomFonction, numeroLigne
 */
uint8_t Send_VTrace(e_type_trace_t Type_Trace, bool Horodatage, const char *i_ps8_nomFichier,
    const char *i_ps8_nomFonction, uint16_t i_u16_numeroLigne, const char *Txt_Donnees, ...);

/**
 * @brief Envoi en trace le contenu d'un buffer (attention taille = 32 octets !!)
 * @param Type_Trace Niveau de trace (pour permettre le filtrage des traces)
 * @param Txt_Donnees Libelle de la donnee a remonter sur la trace
 * @param Buffer Pointeur vers le buffer de donnees a remonter
 * @param Size Taille du buffeur
 * @param Horodatage Demande ou non l'horodatage de la trace
 * @return 0 si OK, 2 si Type de trace trop eleve, 1 si erreurs
 */
uint8_t Send_Trace_Buffer(e_type_trace_t Type_Trace, const char Txt_Donnees[], uint8_t *Buffer,
    uint8_t Size, bool Horodatage = true);

/**
 * @brief Fixe le niveau maximum de trace a remonter
 * @param Level Niveau maximum de trace a afficher
 */
void Set_Max_Debug_Level(e_type_trace_t Level);

/**
 * @brief Retourne le niveau maximum de trace a remonter
 */
e_type_trace_t Get_Max_Debug_Level(void);

/**
 * @brief Test si le niveau de trace est suffisant pour etre ressorti
 * @param Level
 * @return 0 si OK, 1 sinon
 */
uint8_t Test_Trace_Level(e_type_trace_t Level);

/**
 * @brief Renvoi un pointeur vers le texte correspond au niveau de trace
 * @return pointeur vers le texte correspondant, sinon pointeur NULL
 */
const char* Get_Text_Type_Trace(e_type_trace_t Type_Trace);

/**
 * @brief Realise la fonction suivante: sprintf(String,"%lu") sans sprintf
 * @param Num Valeur a convertir en chaine de caracteres
 * @param String Pointeur vers le buffer a ecrire
 * @return Renvoi le nombre de digit ecrit dans String
 */
uint8_t Conv_NumToStr(uint32_t Num, char *String);

void ThreadTxTrace(void *Parametre);

uint8_t DecodeOrdreConfigOrdre(std::stringstream &i_t_TrameADecoder, std::string &o_t_reponse);

#endif /* TRACE_DEBUG_H */
