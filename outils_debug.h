/*
 * outils_debug.h
 *
 *  Created on: 26 sept. 2022
 *      Author: julien
 */

#ifndef OUTILS_DEBUG_H_
#define OUTILS_DEBUG_H_

#define START_MESURE_TEMPS     uint32_t u32_temps = micros()

#define STOP_MESURE_TEMPS      {   u32_temps = micros() - u32_temps; SEND_VTRACE(INFO, "Temps %u us", u32_temps); }

#endif /* OUTILS_DEBUG_H_ */
