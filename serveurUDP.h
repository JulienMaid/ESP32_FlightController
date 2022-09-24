/*
 * serveurUDP.h
 *
 *  Created on: 23 sept. 2022
 *      Author: julien
 */

#pragma once

#include <stdint.h>

void initServeurUDP(uint16_t i_u16_portEcoute);

void theadServeurUDP(void *parametres);
