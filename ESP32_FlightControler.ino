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
#include "LecturePulseCmd.h"
#include "machineetat.h"
#include "actionmachineetat.h"
#include "calculerangles.h"
#include "controleurPID.h"
#include "ES_CI_Proto.h"

Ticker g_t_blinker;
String g_t_MsgDemarrage("ESP32 Flight controller");

ControleurMoteurs *g_pt_ControleurMoteurs = nullptr;
ClassCalculerAngles *g_pt_CalculateurAngles = nullptr;
ControleurPID *g_pt_ControleurPID = nullptr;

QueueHandle_t g_pt_queue = nullptr;

//The setup function is called once at startup of the sketch
void setup()
{
  bool l_b_WifiConnected = false;

  WiFi.setHostname(g_t_MsgDemarrage.c_str());

  // initialisation du Timer matériel pour le module TimerSW
  g_t_blinker.attach(0.05, Inc_Timer);

  l_b_WifiConnected = connecterWifi(0);

  // Initialisation du système de Traces
  Init_Trace_Debug(true, true, std::string(WiFi.broadcastIP().toString().c_str()), 1234);
  Set_Max_Debug_Level(DBG1);

  SEND_VTRACE(INFO, g_t_MsgDemarrage.c_str());

  SEND_VTRACE(INFO, "Connection Wifi: %d", l_b_WifiConnected);

  MachineEtat::retourneInstance()->reglerActionEtat(enum_Etats::ARME, ActionArmement);
  MachineEtat::retourneInstance()->reglerActionEtat(enum_Etats::ATTENTE_ARMEMENT,
      ActionDesarmement);
  MachineEtat::retourneInstance()->reglerActionEtat(enum_Etats::ARRET_URGENCE, ActionArretUrgence);

  // Demarrage du service de mise à jour OTA
  if (l_b_WifiConnected == true)
  {
    DemarrerServeurOTA(g_t_MsgDemarrage);
    SEND_VTRACE(INFO, "Demarrage Service OTA");
  }

  SEND_VTRACE(INFO, "Frequence CPU = %d MHz", getCpuFrequencyMhz());

  initServeurUDP(4321, &g_pt_queue);

  initServeurSerial(&g_pt_queue);

  // Création du calculateur d'angles
  g_pt_CalculateurAngles = new ClassCalculerAngles;

  if (g_pt_CalculateurAngles->Init() != 0)
  {
    SEND_VTRACE(ERROR, "Erreur Intialisation Gyro/Accéléro");
  }

  // Création du correcteur PID
  g_pt_ControleurPID = new ControleurPID;

// Création du module de controle Moteurs
  g_pt_ControleurMoteurs = ControleurMoteurFactory::recupererControleurMoteur(
      e_typeMoteur_t::MOTEUR_BRUSHLESS, ESC1, ESC2, ESC3, ESC4);
  g_pt_ControleurMoteurs->FixerNouvellesConsignePourMille(0, 0, 0, 0);

// Initialisation du décodage des signaux issues du recepteur Radio
  InitPortCmd(Entree1, Entree2, Entree3, Entree4);

  MachineEtat::retourneInstance()->transtionEtatversEtat(enum_Etats::DEMARRAGE,
      enum_Etats::ATTENTE_ARMEMENT);

}

// The loop function is called in an endless loop
void loop()
{
  std::string *l_pt_stringRxUdp = nullptr;
  uint32_t l_u32_TempsUsCourant;
  static uint32_t l_u32_TempsUsPrecedent;

  if (xQueueReceive(g_pt_queue, &l_pt_stringRxUdp, 0) == pdTRUE)
  {
    std::string l_t_localMsg = *l_pt_stringRxUdp;
    delete l_pt_stringRxUdp;

    std::stringstream l_t_Stream(l_t_localMsg);
    std::string l_t_Arg1;
    std::string l_t_Reponse;

    SEND_VTRACE(INFO, "Core %d; RX dans loop: %s", xPortGetCoreID(), l_t_localMsg.c_str());

    l_t_Stream >> l_t_Arg1;

    if (l_t_Arg1 == "Traces")
    {
      DecodeOrdreConfigOrdre(l_t_Stream, l_t_Reponse);
    }
    else
    {
      if (l_t_Arg1 == "OFF")
      {
        SEND_VTRACE(INFO, "Moteurs OFF");
        MachineEtat::retourneInstance()->transtionEtatversEtat(enum_Etats::ARME,
            enum_Etats::ARRET_URGENCE);

      }
      else if (l_t_Arg1 == "ON")
      {

        uint16_t u16_Val = 0;
        sscanf((const char*) l_t_localMsg.c_str(), "ON %d", &u16_Val);
        SEND_VTRACE(INFO, "Valeur Moteur = %d %", u16_Val);

        g_tu32_ImpulsionVoies[e_NumeroVoie_t::Voie3] = map(u16_Val, 0, 100, 1000, 2000);

        MachineEtat::retourneInstance()->transtionEtatversEtat(enum_Etats::ATTENTE_ARMEMENT,
            enum_Etats::ARME);
      }
    }
  }

  if (g_tu32_ImpulsionVoies[e_NumeroVoie_t::Voie5] > 1600)
  {
    MachineEtat::retourneInstance()->transtionEtatversEtat(enum_Etats::ATTENTE_ARMEMENT,
        enum_Etats::ARME);
  }
  else if (g_tu32_ImpulsionVoies[e_NumeroVoie_t::Voie5] < 1400)
  {
    MachineEtat::retourneInstance()->transtionEtatversEtat(enum_Etats::ARME,
        enum_Etats::ATTENTE_ARMEMENT);
  }

  if (MachineEtat::retourneInstance()->retourneEtatInterne() == enum_Etats::ARRET_URGENCE)
  {
    g_pt_ControleurMoteurs->FixerNouvellesConsignePourMille(0, 0, 0, 0);
  }
  else if (MachineEtat::retourneInstance()->retourneEtatInterne() == enum_Etats::ARME)
  {
    l_u32_TempsUsCourant = micros();

    // Gestion de l'overflow de micros()
    if (l_u32_TempsUsCourant < l_u32_TempsUsPrecedent)
    {
      l_u32_TempsUsPrecedent = l_u32_TempsUsCourant;
      SEND_VTRACE(INFO, "OverFlow micros()");
    }

    if (l_u32_TempsUsCourant >= (l_u32_TempsUsPrecedent + 4000))
    {
      static uint8_t l_u8_tempoTraces = 0;

      float l_tf_mesures[e_ListeMouvements_t::NbreMouvements];
      float l_tf_DeplacementsAngulaires[e_ListeMouvements_t::NbreMouvements];

      uint16_t l_u16_MoteurAvG = 0;
      uint16_t l_u16_MoteurAvD = 0;
      uint16_t l_u16_MoteurArG = 0;
      uint16_t l_u16_MoteurArD = 0;

      l_u32_TempsUsPrecedent = l_u32_TempsUsCourant;

      if (g_pt_CalculateurAngles->NouvellesValeursMPU6050() != 0)
      {
        SEND_VTRACE(ERROR, "Erreur Lecture MPU6050");
      }

      g_pt_CalculateurAngles->CalculerAngles();

      g_pt_CalculateurAngles->DonnerMesures(l_tf_mesures);
      g_pt_CalculateurAngles->DonnerDeplacementsAngulaires(l_tf_DeplacementsAngulaires);

      g_pt_ControleurPID->NouvellesValeursMesures(l_tf_mesures[e_ListeMouvements_t::Yaw],
          l_tf_mesures[e_ListeMouvements_t::Pitch], l_tf_mesures[e_ListeMouvements_t::Roll]);

      g_pt_ControleurPID->NouvellesValeursMouvementsAngulaires(
          l_tf_DeplacementsAngulaires[e_ListeMouvements_t::Yaw],
          l_tf_DeplacementsAngulaires[e_ListeMouvements_t::Pitch],
          l_tf_DeplacementsAngulaires[e_ListeMouvements_t::Roll]);

      g_pt_ControleurPID->NouvellesValeursCommandes(g_tu32_ImpulsionVoies[e_NumeroVoie_t::Voie1],
          g_tu32_ImpulsionVoies[e_NumeroVoie_t::Voie2],
          g_tu32_ImpulsionVoies[e_NumeroVoie_t::Voie3],
          g_tu32_ImpulsionVoies[e_NumeroVoie_t::Voie4]);

      g_pt_ControleurPID->RecupererNouvellesConsignesMoteurs(l_u16_MoteurAvG, l_u16_MoteurAvD,
          l_u16_MoteurArG, l_u16_MoteurArD);

      if (l_u8_tempoTraces != 0)
      {
        l_u8_tempoTraces--;
      }
      else
      {
        l_u8_tempoTraces = 40;

        SEND_VTRACE(INFO,
            "Depl Yaw: %4.3f - Depl Pitch: %4.3f - Depl Roll: %4.3f Mes Yaw: %4.3f - Mes Pitch: %4.3f - Mes Roll: %4.3f",
            l_tf_DeplacementsAngulaires[e_ListeMouvements_t::Yaw],
            l_tf_DeplacementsAngulaires[e_ListeMouvements_t::Pitch],
            l_tf_DeplacementsAngulaires[e_ListeMouvements_t::Roll],
            l_tf_mesures[e_ListeMouvements_t::Yaw], l_tf_mesures[e_ListeMouvements_t::Pitch],
            l_tf_mesures[e_ListeMouvements_t::Roll]);
      }
    }

  }
}
