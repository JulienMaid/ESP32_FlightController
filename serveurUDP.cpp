#include "serveurUDP.h"
#include <Arduino.h>
#include <WiFi.h>
#include "trace_debug.h"

AsyncUDP udp;
char tc_RxBuffer[255];
bool g_b_threadActif;

void initServeurUDP(uint16_t i_u16_portEcoute)
{
  if (udp.listen(i_u16_portEcoute))
  {
    SEND_VTRACE(INFO, "Ecoute UDP sur IP: %s", WiFi.localIP().toString().c_str());
    udp.onPacket(onPacketCallBack);
  }
}

void onPacketCallBack(AsyncUDPPacket packet)
{
  Serial.write(packet.data(), packet.length());
  Serial.println(xPortGetCoreID());
  packet.flush();
  Serial.println();
}
//void theadServeurUDP(void *parametres)
//{
//  while (g_b_threadActif)
//  {
//    uint16_t l_u16_taillePaquet = udp.parsePacket();
//    if (l_u16_taillePaquet)
//    {
//      uint16_t l_u16_tailleMessage = udp.read(tc_RxBuffer, sizeof(tc_RxBuffer));
//      if (l_u16_tailleMessage > 0)
//        Serial.printf("Data : %s\n", tc_RxBuffer);
//
//    }
//    delay(100);
//  }
//
//  vTaskDelete(NULL);
//}
