#include "serveurUDP.h"
#include <Arduino.h>
#include <WiFi.h>
#include "trace_debug.h"

AsyncUDP udp;
//char tc_RxBuffer[255];
//bool g_b_threadActif;

QueueHandle_t *g_ppt_queue;

uint8_t initServeurUDP(uint16_t i_u16_portEcoute, QueueHandle_t *i_ppt_queue)
{
  uint8_t u8_CR = -1;

  if (udp.listen(i_u16_portEcoute))
  {
    SEND_VTRACE(INFO, "Ecoute UDP sur IP: %s", WiFi.localIP().toString().c_str());
    udp.onPacket(onPacketCallBack);

    if (*i_ppt_queue == nullptr)
    {
      *i_ppt_queue = xQueueCreate(10, sizeof(std::string*));
      SEND_VTRACE(DBG1, "Init Queue");
    }

    g_ppt_queue = i_ppt_queue;

    u8_CR = 0;
  }

  return u8_CR;
}

void onPacketCallBack(AsyncUDPPacket packet)
{
  std::string *l_pt_stringRxUdp;

  l_pt_stringRxUdp = new std::string((const char*) packet.data(), packet.length());

  SEND_VTRACE(DBG2, "Core %d; RX: %s", xPortGetCoreID(), l_pt_stringRxUdp->c_str());

  xQueueSend(*g_ppt_queue, &l_pt_stringRxUdp, 0);

  packet.flush();
}
