/*
 * serveurSerial.h
 *
 *  Created on: 8 nov. 2022
 *      Author: julien
 */

#ifndef SERVEURSERIAL_H_
#define SERVEURSERIAL_H_

#include <stdint.h>
#include <Arduino.h>

uint8_t initServeurSerial(QueueHandle_t *i_ppt_queue);

void onSerialCallBack(void);

#endif /* SERVEURSERIAL_H_ */
