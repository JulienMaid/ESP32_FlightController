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
#include "controleurMoteursFactory.h"
#include "LecturePulseCmd.h"

Ticker g_t_blinker;
String g_t_MsgDemarrage("ESP32 Flight controller");

ControleurMoteurs *g_pt_ControleurMoteurs = nullptr;

//The setup function is called once at startup of the sketch
void setup()
{
  bool l_b_WifiConnected = false;

  // Initialisation du Timer matériel pour le module TimerSW
  g_t_blinker.attach(0.05, Inc_Timer);

  // Initialisation du système de Traces
  Init_Trace_Debug();
  Set_Max_Debug_Level(DBG1);

  l_b_WifiConnected = connecterWifi(0);

  SEND_VTRACE(INFO, g_t_MsgDemarrage.c_str());

  SEND_VTRACE(INFO, "Connection Wifi: %d", l_b_WifiConnected);

  // Demarrage du service de mise à jour OTA
  if (l_b_WifiConnected == true)
  {
    DemarrerServeurOTA(g_t_MsgDemarrage);
    SEND_VTRACE(INFO, "Demarrage Service OTA");
  }

  SEND_VTRACE(INFO, "Frequence CPU = %d MHz", getCpuFrequencyMhz());

  // Initialisation du serveur UDP pour recevoir des commandes en Wifi
  initServeurUDP(4321);

  // Création du module de controle Moteurs
  g_pt_ControleurMoteurs = ControleurMoteurFactory::recupererControleurMoteur(
      e_typeMoteur_t::MOTEUR_BRUSHLESS, 2, 4, 16, 17);

  // Initialisation du décodage des signaux issues du recepteur Radio
  InitPortCmd();

}

// The loop function is called in an endless loop
void loop()
{
  std::string *l_pt_stringRxUdp = NULL;
  static uint32_t u32_TempsUsPrecedent = micros();
  uint32_t u32_TempsUsCourant;
  static bool l_b_MoteursActives = false;

  uint8_t var = 8;

  if (xQueueReceive(g_pt_queue, &l_pt_stringRxUdp, 0) == pdTRUE)
  {
    std::string l_t_localMsg = *l_pt_stringRxUdp;
    delete l_pt_stringRxUdp;

    SEND_VTRACE(INFO, "Core %d; RX dans loop: %s", xPortGetCoreID(), l_t_localMsg.c_str());

    if (l_t_localMsg == "OFF")
    {
      SEND_VTRACE(INFO, "Moteurs OFF");
      g_pt_ControleurMoteurs->FixerNouvellesConsigne(0, 0, 0, 0);
      l_b_MoteursActives = false;
    }
    else if (l_t_localMsg >= "ON")
    {

      uint16_t u16_Val = 0;
      sscanf((const char*) l_t_localMsg.c_str(), "ON %d", &u16_Val);

      SEND_VTRACE(INFO, "Valeur Moteur = %d %", u16_Val);

      g_pt_ControleurMoteurs->FixerNouvellesConsigne(20, 40, 60, 80);
      l_b_MoteursActives = true;
    }
  }

  u32_TempsUsCourant = micros();

  // Gestion de l'overflow de micros()
  if (u32_TempsUsCourant < u32_TempsUsPrecedent)
  {
    u32_TempsUsPrecedent = u32_TempsUsCourant;
    SEND_VTRACE(INFO, "OverFlow micros()");
  }

  if ((u32_TempsUsCourant - u32_TempsUsPrecedent) >= 4000000)
  {
    u32_TempsUsPrecedent = u32_TempsUsCourant;

    SEND_VTRACE(INFO, "TOP !");

  }

}
