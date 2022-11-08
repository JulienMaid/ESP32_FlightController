#include "serveurUDP.h"
#include <Arduino.h>
#include <WiFi.h>
#include "trace_debug.h"

AsyncUDP udp;
//char tc_RxBuffer[255];
//bool g_b_threadActif;
QueueHandle_t g_pt_queue = nullptr;

uint8_t initServeurUDP(uint16_t i_u16_portEcoute)
{
  uint8_t u8_CR = -1;

  if (udp.listen(i_u16_portEcoute))
  {
    SEND_VTRACE(INFO, "Ecoute UDP sur IP: %s", WiFi.localIP().toString().c_str());
    udp.onPacket(onPacketCallBack);

    g_pt_queue = xQueueCreate(10, sizeof(std::string*));

    u8_CR = 0;
  }

  return u8_CR;
}

void onPacketCallBack(AsyncUDPPacket packet)
{
  std::string *l_pt_stringRxUdp;

  l_pt_stringRxUdp = new std::string((const char*) packet.data(), packet.length());

  SEND_VTRACE(DBG2, "Core %d; RX: %s", xPortGetCoreID(), l_pt_stringRxUdp->c_str());

  xQueueSend(g_pt_queue, &l_pt_stringRxUdp, 0);

  packet.flush();
}
