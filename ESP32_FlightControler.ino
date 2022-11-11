#include <Arduino.h>
#include <WiFi.h>
#include <Ticker.h>
#include "timer_sw.h"
#include "super_timer_sw.h"
#include "RTC_soft.h"
#include "trace_debug.h"
#include "outils_wifi.h"
#include "OTAUpdate.h"
#include "serveurUDP.h"
#include "serveurSerial.h"
#include "controleurMoteursFactory.h"

Ticker g_t_blinker;
String g_t_MsgDemarrage("ESP32 Flight controller");

ControleurMoteurs *g_pt_ControleurMoteurs = nullptr;

QueueHandle_t g_pt_queue = nullptr;

//The setup function is called once at startup of the sketch
void setup()
{
  bool l_b_WifiConnected = false;

  // initialisation du Timer matériel pour le module TimerSW
  g_t_blinker.attach(0.05, Inc_Timer);

  Init_Trace_Debug();
  Set_Max_Debug_Level(DBG1);

  l_b_WifiConnected = connecterWifi(0);

  SEND_VTRACE(INFO, g_t_MsgDemarrage.c_str());

  SEND_VTRACE(INFO, "Connection Wifi: %d", l_b_WifiConnected);

  if (l_b_WifiConnected == true)
  {
    DemarrerServeurOTA(g_t_MsgDemarrage);
    SEND_VTRACE(INFO, "Demarrage Service OTA");
  }

  SEND_VTRACE(INFO, "Frequence CPU = %d MHz", getCpuFrequencyMhz());

  initServeurUDP(4321, &g_pt_queue);

  initServeurSerial(&g_pt_queue);

//  g_pt_ControleurMoteurs = ControleurMoteurFactory::recupererControleurMoteur(
//      e_typeMoteur_t::MOTEUR_BRUSHLESS, 2, 4, 16, 17);

  g_pt_ControleurMoteurs = ControleurMoteurFactory::recupererControleurMoteur(
      e_typeMoteur_t::MOTEUR_BRUSHED, 2, 4, 16, 17);

}

// The loop function is called in an endless loop
void loop()
{
  std::string *l_pt_stringRxUdp = nullptr;

  if (xQueueReceive(g_pt_queue, &l_pt_stringRxUdp, 0) == pdTRUE)
  {
    std::string l_t_localMsg = *l_pt_stringRxUdp;
    delete l_pt_stringRxUdp;

    std::stringstream l_t_Stream(l_t_localMsg);
    std::string l_t_Arg1;

    SEND_VTRACE(INFO, "Core %d; RX dans loop: %s", xPortGetCoreID(), l_t_localMsg.c_str());

    l_t_Stream >> l_t_Arg1;

    if (l_t_Arg1 == "Traces")
    {
      DecodeOrdreConfigOrdre(l_t_Stream);
    }
    else
    {
      if (l_t_Arg1 == "OFF")
      {
        SEND_VTRACE(INFO, "Moteurs OFF");
      }
      else if (l_t_Arg1 == "ON")
      {

        uint16_t u16_Val = 0;
        sscanf((const char*) l_t_localMsg.c_str(), "ON %d", &u16_Val);

        SEND_VTRACE(INFO, "Valeur Moteur = %d %", u16_Val);

        g_pt_ControleurMoteurs->FixerNouvellesConsignePourMille(20, 40, 60, 80);
      }
    }
  }

  delay(100);

//  SEND_VTRACE(INFO, "Core %d, Test2", xPortGetCoreID());
}
