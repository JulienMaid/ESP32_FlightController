/*
 * ConstantesPID.h
 *
 *  Created on: 4 oct. 2022
 *      Author: julien
 */

#ifndef CONSTANTESPID_H_
#define CONSTANTESPID_H_

#include <stdint.h>

enum e_RepereOrthonormal_t
{
  X = 0, Y = 1, Z = 2
};

enum e_ListeVoies_t
{
  Voie1 = 0, Voie2 = 1, Voie3 = 2, Voie4 = 3, NbreVoies
};

enum e_ListeMouvements_t : uint8_t
{
  Yaw = 0, Pitch = 1, Roll = 2, Throttle = 3, NbreMouvements
};

#endif /* CONSTANTESPID_H_ */
