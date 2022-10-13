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
#include "calculerangles.h"

Ticker g_t_blinker;
String g_t_MsgDemarrage("ESP32 Flight controller");

ControleurMoteurs *g_pt_ControleurMoteurs = nullptr;
ClassCalculerAngles *g_pt_CalculateurAngles = nullptr;

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

  initServeurUDP(4321);

//  g_pt_ControleurMoteurs = ControleurMoteurFactory::recupererControleurMoteur(
//      e_typeMoteur_t::MOTEUR_BRUSHLESS, 2, 4, 16, 17);

  g_pt_ControleurMoteurs = ControleurMoteurFactory::recupererControleurMoteur(
      e_typeMoteur_t::MOTEUR_BRUSHED, 2, 4, 16, 17);

  g_pt_CalculateurAngles = new ClassCalculerAngles;

}

// The loop function is called in an endless loop
void loop()
{
  static uint32_t l_u32_TempsPrecedent = micros();
  uint32_t l_u32_TempsCourant;
  std::string *l_pt_stringRxUdp = NULL;

  uint8_t var = 8;

  if (xQueueReceive(g_pt_queue, &l_pt_stringRxUdp, 0) == pdTRUE)
  {
    std::string l_t_localMsg = *l_pt_stringRxUdp;
    delete l_pt_stringRxUdp;

    SEND_VTRACE(INFO, "Core %d; RX dans loop: %s", xPortGetCoreID(), l_t_localMsg.c_str());

    if (l_t_localMsg == "OFF")
    {
      SEND_VTRACE(INFO, "Moteurs OFF");
    }
    else if (l_t_localMsg >= "ON")
    {

      uint16_t u16_Val = 0;
      sscanf((const char*) l_t_localMsg.c_str(), "ON %d", &u16_Val);

      SEND_VTRACE(INFO, "Valeur Moteur = %d %", u16_Val);

      g_pt_ControleurMoteurs->FixerNouvellesConsignePourMille(20, 40, 60, 80);
    }
  }

  l_u32_TempsCourant = micros();

  // Gestion overflow de micros(), se produit toutes les 1h10
  if (l_u32_TempsCourant < l_u32_TempsPrecedent)
  {
    //TODO A gérer !!
    l_u32_TempsPrecedent = l_u32_TempsCourant;
  }

  if (l_u32_TempsCourant >= (l_u32_TempsPrecedent + 4000))
  {
    static uint8_t l_u8_tempoTraces = 0;

    float l_tf_mesures[e_ListeMouvements_t::NbreMouvements];
    float l_tf_DeplacementsAngulaires[e_ListeMouvements_t::NbreMouvements];

    l_u32_TempsPrecedent = l_u32_TempsCourant;

    g_pt_CalculateurAngles->NouvellesValeursMPU6050();

    g_pt_CalculateurAngles->CalculerAngles();

    g_pt_CalculateurAngles->DonnerMesures(l_tf_mesures);
    g_pt_CalculateurAngles->DonnerDeplacementsAngulaires(l_tf_DeplacementsAngulaires);

    if (l_u8_tempoTraces != 0)
    {
      l_u8_tempoTraces--;
    }
    else
    {
      l_u8_tempoTraces = 40;

      SEND_VTRACE(INFO, "Yaw: %4.3f - Pitch: %4.3f - Roll: %4.3f",
          l_tf_DeplacementsAngulaires[e_ListeMouvements_t::Yaw],
          l_tf_DeplacementsAngulaires[e_ListeMouvements_t::Pitch],
          l_tf_DeplacementsAngulaires[e_ListeMouvements_t::Roll]);
    }
  }

//  delay(100);

//  SEND_VTRACE(INFO, "Core %d, Test2", xPortGetCoreID());
}
