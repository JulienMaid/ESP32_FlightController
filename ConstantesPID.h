/*
 * ConstantesPID.h
 *
 *  Created on: 4 oct. 2022
 *      Author: julien
 */

#ifndef CONSTANTESPID_H_
#define CONSTANTESPID_H_

#include <stdint.h>

enum e_RepereOrthonormal_t : uint8_t
{
  X = 0, Y = 1, Z = 2, NbreAxes
};

enum e_ListeVoies_t : uint8_t
{
  VoieNum1 = 0, VoieNum2 = 1, VoieNum3 = 2, VoieNum4 = 3, NbreVoiesPID
};

enum e_ListeMouvements_t : uint8_t
{
  Yaw = 0, Pitch = 1, Roll = 2, Throttle = 3, NbreMouvements
};

#endif /* CONSTANTESPID_H_ */
