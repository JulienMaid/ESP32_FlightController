#include "serveurUDP.h"
#include <WiFiUdp.h>

WiFiUDP udp;
char tc_RxBuffer[255];
bool g_b_threadActif;

void initServeurUDP(uint16_t i_u16_portEcoute)
{
  udp.begin(i_u16_portEcoute);
  g_b_threadActif = true;

  xTaskCreate(theadServeurUDP, /* Task function. */
  "theadServeurUDP", /* String with name of task. */
  10000, /* Stack size in words. */
  NULL, /* Parameter passed as input of the task */
  2, /* Priority of the task. */
  NULL);

//  xTaskCreatePinnedToCore(theadServeurUDP, /* Function to implement the task */
//  "coreTask", /* Name of the task */
//  10000, /* Stack size in words */
//  NULL, /* Task input parameter */
//  0, /* Priority of the task */
//  NULL, /* Task handle. */
//  0);

}

void theadServeurUDP(void *parametres)
{
  while (g_b_threadActif)
  {
    uint16_t l_u16_taillePaquet = udp.parsePacket();
    Serial.print(" Received packet from : ");
    Serial.println(udp.remoteIP());
    Serial.print(" Size : ");
    Serial.println(l_u16_taillePaquet);
    if (l_u16_taillePaquet)
    {
      uint16_t l_u16_tailleMessage = udp.read(tc_RxBuffer, sizeof(tc_RxBuffer));
      if (l_u16_tailleMessage > 0)
        tc_RxBuffer[l_u16_tailleMessage - 1] = 0;
      Serial.printf("Data : %s\n", tc_RxBuffer);
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.printf("UDP packet was received OK\r\n");
      udp.endPacket();

    }
    delay(100);
  }

  vTaskDelete(NULL);
}
