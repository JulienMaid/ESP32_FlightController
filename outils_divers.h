/*
 * outils_divers.h
 *
 *  Created on: 9 oct. 2022
 *      Author: julien
 */

#ifndef OUTILS_DIVERS_H_
#define OUTILS_DIVERS_H_

/**
 * Limiter une valeur entre deux bornes
 *
 * @param T valeur     : Valeur à plafonner
 * @param T valeurMini : Valeur minimum
 * @param T valeurMaxi : Valeur maximum
 *
 * @return T
 */
template<typename T>
T minMax(T valeur, T valeurMini, T valeurMaxi);

#endif /* OUTILS_DIVERS_H_ */
