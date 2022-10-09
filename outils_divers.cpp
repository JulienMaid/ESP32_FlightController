/*
 * outils_divers.cpp
 *
 *  Created on: 9 oct. 2022
 *      Author: julien
 */

#include "outils_divers.h"

template<typename T>
T minMax(T valeur, T valeurMini, T valeurMaxi)
{
  if (valeur > valeurMaxi)
  {
    valeur = valeurMaxi;
  }
  else if (valeur < valeurMini)
  {
    valeur = valeurMini;
  }

  return valeur;
}
