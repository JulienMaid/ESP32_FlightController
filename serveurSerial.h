/*
 * serveurSerial.h
 *
 *  Created on: 8 nov. 2022
 *      Author: julien
 */

#ifndef SERVEURSERIAL_H_
#define SERVEURSERIAL_H_

#include <stdint.h>

uint8_t initServeurSerial(void);

void onSerialCallBack(void);

#endif /* SERVEURSERIAL_H_ */
