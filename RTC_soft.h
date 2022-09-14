/// @file RTC_soft.h
/// <b>Date de creation:</b> 16/12/2015                                                      <br>
///                                                                                          <br>
/// @brief Implementation d'une pseudo RTC pour horodater les trames de trace                <br>
///                                                                                          <br>
///==========================================================================================<br>
/// <b>Donnees SVN:</b>                                                                      <br>
/// $Rev:: 148                                                                              $<br>
/// $Author:: julien                                                                        $<br>
/// $Date:: 2016-01-18 14:25:08 +0100 (lun., 18 janv. 2016)                                  $<br>
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

#ifndef RTC_SOFT_H
#define RTC_SOFT_H

#include <stdint.h>

#define UTILISER_TIMER_SW_PR_RTC_SOFT		1

//*************************************
//* Prototype des fonctions publiques *
//*************************************

/**
 * @fn void Init_RTC_Soft(void)
 * @brief Initialise la pseudo RTC (T = 100ms)
 */
void Init_RTC_Soft(void);

/**
 * @fn uint32_t Get_Time(void)
 * @brief Renvoie le temps depuis l'initialisation
 * @return Temps en cours en x100ms
 */
uint32_t Get_Time(void);

/**
 * @fn void IRQ_Inc_RTC_Soft(void)
 * @brief Fonction Handler de l'interruption faisant s'incrementer la RTC
 */
void IRQ_Inc_RTC_Soft(uint32_t Val, void *Arg);

#endif /* RTC_SOFT_H */
