/*
 * serveurUDP.h
 *
 *  Created on: 23 sept. 2022
 *      Author: julien
 */

#pragma once

#include <stdint.h>
#include <AsyncUDP.h>

uint8_t initServeurUDP(uint16_t i_u16_portEcoute, QueueHandle_t *i_ppt_queue);

void onPacketCallBack(AsyncUDPPacket packet);

void theadServeurUDP(void *parametres);
