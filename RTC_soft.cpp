// @file RTC_soft.c
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

#define RTC_SOFT_C

//************************************
//* Includes lies a l'implementation *
//************************************

#include "RTC_soft.h"

#if UTILISER_TIMER_SW_PR_RTC_SOFT == 1
#include "timer_sw.h"
#else // UTILISER_TIMER_SW_PR_RTC_SOFT == 1

#endif // UTILISER_TIMER_SW_PR_RTC_SOFT == 1

#include <stdint.h>
//*********************
//* Variables privees *
//*********************

/// @brief Compteur de la RTC
uint32_t Val_RTC_Soft = 0;

TimerEvent_t Timer_RTC_Soft;

//********************************
//* Implementation des fonctions *
//********************************

void Init_RTC_Soft(void)
{
	Val_RTC_Soft = 0;

  Timer_RTC_Soft.Init(IRQ_Inc_RTC_Soft, 100, Periodic_Timer, NULL, NULL);
	Timer_RTC_Soft.Start();
}

uint32_t Get_Time(void)
{
	return Val_RTC_Soft;
}

void IRQ_Inc_RTC_Soft(uint32_t Val, void *Arg)
{
  (void) Val;
  (void) Arg;

	Val_RTC_Soft++;
}

