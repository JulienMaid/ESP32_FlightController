/// @file trace_debug.h
/// <b>Date de creation:</b> 16/12/2015                                                      <br>
///                                                                                          <br>
/// @brief Bank de texte a afficher dans la trace                                            <br>
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

#ifndef BANK_TEXT_DEBUG_H_
#define BANK_TEXT_DEBUG_H_

#include <stdint.h>
#ifdef TRACE_DEBUG_C

const uint8_t Txt_None[] = "NONE";
const uint8_t Txt_Error[] = "ERROR";
const uint8_t Txt_Info[] = "INFO";
const uint8_t Txt_Tst_Secu_Results[] = "TSR";
const uint8_t Txt_Debug1[] = "DBG1";
const uint8_t Txt_Debug2[] = "DBG2";
const uint8_t Txt_Debug3[] = "DBG3";
const uint8_t Txt_Debug4[] = "DBG4";
const uint8_t Txt_DebugSpecif[] = "DBGX";
const uint8_t Txt_All[] = "ALL";

const uint8_t Txt_SF12[] = "SF12";
const uint8_t Txt_SF11[] = "SF11";
const uint8_t Txt_SF10[] = "SF10";
const uint8_t Txt_SF09[] = "SF09";
const uint8_t Txt_SF08[] = "SF08";
const uint8_t Txt_SF07[] = "SF07";
const uint8_t Txt_SF07H[] = "SF07H";

const uint8_t * Txt_SFx[] =
{
		Txt_SF12,
		Txt_SF11,
		Txt_SF10,
		Txt_SF09,
		Txt_SF08,
		Txt_SF07,
		Txt_SF07H
};

const uint8_t DC000[] = "DC 0.0%";
const uint8_t DC0_1[] = "DC 0.1%";
const uint8_t DC01_0[] = "DC 1.0%";
const uint8_t DC010_0[] = "DC 10%";
const uint8_t DC100_0[] = "DC 100%";

const uint8_t * Txt_DC[] =
{
		DC000,
		DC0_1,
		DC01_0,
		DC010_0,
		DC100_0
};

#else // TRACE_DEBUG_C

extern const uint8_t Txt_None[];
extern const uint8_t Txt_Error[];
extern const uint8_t Txt_Info[];
extern const uint8_t Txt_Tst_Secu_Results[];
extern const uint8_t Txt_Debug1[];
extern const uint8_t Txt_Debug2[];
extern const uint8_t Txt_Debug3[];
extern const uint8_t Txt_Debug4[];
extern const uint8_t Txt_DebugSpecif[];
extern const uint8_t Txt_All[];

extern const uint8_t * Txt_SFx[];

extern const uint8_t * Txt_DC[];

#endif // TRACE_DEBUG_C

#endif /* BANK_TEXT_DEBUG_H_ */
