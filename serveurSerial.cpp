#include "serveurSerial.h"
#include <Arduino.h>

extern QueueHandle_t g_pt_queue;

uint8_t initServeurSerial(void)
{
  uint8_t u8_CR = -1;

  Serial.onReceive(onSerialCallBack);

  return u8_CR;
}

void onSerialCallBack(void)
{
  char l_ts8_BufferRx[50];
  uint16_t l_u16_octetsRecus = 0;
  static std::string l_t_StringRX;
  std::string *l_t_StringQueue;

  if (Serial.available())
  {
    l_u16_octetsRecus = Serial.read(l_ts8_BufferRx, sizeof(l_ts8_BufferRx));

    if ((l_ts8_BufferRx[l_u16_octetsRecus - 1] == '\n')
        || (l_ts8_BufferRx[l_u16_octetsRecus - 1] == '\r'))
    {
      l_ts8_BufferRx[l_u16_octetsRecus] = 0;
      Serial.print(l_ts8_BufferRx);
      l_ts8_BufferRx[l_u16_octetsRecus - 1] = 0;
      l_t_StringRX.append(l_ts8_BufferRx);
      l_t_StringQueue = new std::string(l_t_StringRX);
      xQueueSend(g_pt_queue, &l_t_StringQueue, 0);
      l_t_StringRX.clear();
    }
    else
    {
      l_ts8_BufferRx[l_u16_octetsRecus] = 0;
      l_t_StringRX.append(l_ts8_BufferRx);
      Serial.print(l_ts8_BufferRx);
    }
  }
}
