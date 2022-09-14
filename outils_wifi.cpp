/*
 * outils_wifi.cpp
 *
 *  Created on: 4 sept. 2022
 *      Author: julien
 */

#include "outils_wifi.h"

#include <Arduino.h>

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#error definir ESP32 ou ESP8266
#endif

#include "timer_sw.h"

s_configReseau_t ts_configReseau[] = { { "NETGEAR11", "1a2b3c4d5e6f7g8h9i0j", "192.168.79.10" },

{ "Usine", "nainporte", "192.168.79.10" } };

uint8_t u8_tailleConfigReseau = sizeof(ts_configReseau) / sizeof(s_configReseau_t);

bool connecterWifi(uint8_t i_u8_NumeroPortClignote)
{
  bool e_connecte = false;

  TimerEvent_t TempoConnection(NULL, 4000, false);
  uint8_t u8_indice = 0;
  uint8_t u8_maxindice = 1;
  uint16_t u16_nbreReseauWifi = 0;

  pinMode(i_u8_NumeroPortClignote, OUTPUT);

#if defined(ESP_32)
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
#endif

//  for (uint8_t u8_index = 0; u8_index < u8_tailleConfigReseau; u8_index++)
//  {
//    TempoConnection.Start();
//    WiFi.begin(ts_configReseau[u8_index].ts8_ssid, ts_configReseau[u8_index].ts8_password);
//
//    while ((WiFi.status() != WL_CONNECTED) && (TempoConnection.IsTop() == false))
//    {
//      delay(250);
//      digitalWrite(i_u8_NumeroPortClignote, 1);
//      delay(250);
//      digitalWrite(i_u8_NumeroPortClignote, 0);
//
//      Serial.print(".");
//    }
//
//    TempoConnection.Delete();
//
//    if (WiFi.status() == WL_CONNECTED)
//    {
//
//      return true;
//    }
//
//  }

  u16_nbreReseauWifi = WiFi.scanNetworks();
  if (u16_nbreReseauWifi == 0)
  {
    Serial.println("no networks found");
    return false;
  }
  else
  {
    Serial.print(u16_nbreReseauWifi);
    Serial.println(" networks found");
    for (uint16_t u16_indexBcle = 0; u16_indexBcle < u16_nbreReseauWifi; u16_indexBcle++)
    {
      // Print SSID and RSSI for each network found
      Serial.print(u16_indexBcle + 1);
      Serial.print(": ");
      Serial.println(WiFi.SSID(u16_indexBcle));
      delay(10);

      for (uint8_t u8_index = 0; u8_index < u8_tailleConfigReseau; u8_index++)
      {
        if (WiFi.SSID(u16_indexBcle) == ts_configReseau[u8_index].ts8_ssid)
        {
          TempoConnection.Start();
          WiFi.begin(ts_configReseau[u8_index].ts8_ssid, ts_configReseau[u8_index].ts8_password);

          while ((WiFi.status() != WL_CONNECTED) && (TempoConnection.IsTop() == false))
          {
            delay(250);
            digitalWrite(i_u8_NumeroPortClignote, 1);
            delay(250);
            digitalWrite(i_u8_NumeroPortClignote, 0);

            Serial.print(".");
          }

          TempoConnection.Delete();
          Serial.println("");

          if (WiFi.status() == WL_CONNECTED)
          {
            String AdresseIP(WiFi.localIP().toString());
            Serial.print("Adresse IP:");
            Serial.println(AdresseIP);
            return true;
          }
          else
          {
            return false;
          }

        }
      }
    }
  }
  return false;
}
