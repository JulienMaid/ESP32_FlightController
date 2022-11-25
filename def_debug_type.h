/// @file def_debug_type.h
/// <b>Date de creation:</b> 05/01/2016                                                      <br>
///                                                                                          <br>
/// @brief Definition des types enum utilises dans les routines de traces                    <br>
///                                                                                          <br>
///==========================================================================================<br>
/// <b>Donnees SVN:</b>                                                                      <br>
/// $Rev:: 602                                                                              $<br>
/// $Author:: julien                                                                        $<br>
/// $Date:: 2017-03-27 13:51:51 +0200 (lun., 27 mars 2017)                                   $<br>
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

#ifndef DEF_DEBUG_TYPE_H_
#define DEF_DEBUG_TYPE_H_

// Definition des niveaux possibles de traces
typedef enum def_type_trace : uint8_t
{
  NONE = 0, ERROR, WARNING, INFO, TEST_SECU_RESULTS, DBG1, DBG2, DBG3, DBG4, DBGX = 0xfe, ALL = 0xff
} e_type_trace_t;

// Declaration de #define pour la retro-compatibilite avec les versions precedentes
#define DEBUG1			DBG1
#define DEBUG2			DBG2
#define DEBUG3			DBG3
#define DEBUG4			DBG4
#define DEBUG_SPECIAL	DBGX

// Definition des evenements timers
typedef enum def_type_timer
{
  TIMER_INIT = 0,
  TIMER_START,
  TIMER_STOP,
  TIMER_HALT,
  TIMER_TOP,
  TIMER_FINTOP,
  TIMER_DELETE,
  TIMER_SETVALUE
} type_timer_t;

#endif /* DEF_DEBUG_TYPE_H_ */
