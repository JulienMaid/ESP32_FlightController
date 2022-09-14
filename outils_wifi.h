#pragma once

#include <stdint.h>

typedef struct
{
  char ts8_ssid[20];
  const char ts8_password[30];
  const char ts8_ipServeur[30];
} s_configReseau_t;

extern s_configReseau_t ts_configReseau[];
extern uint8_t u8_tailleConfigReseau;

bool connecterWifi(uint8_t i_u8_NumeroPortClignote);
