/*
 * serveurUDP.h
 *
 *  Created on: 23 sept. 2022
 *      Author: julien
 */

#pragma once

#include <stdint.h>
#include <AsyncUDP.h>

extern QueueHandle_t g_pt_queue;

uint8_t initServeurUDP(uint16_t i_u16_portEcoute);

void onPacketCallBack(AsyncUDPPacket packet);

void theadServeurUDP(void *parametres);
