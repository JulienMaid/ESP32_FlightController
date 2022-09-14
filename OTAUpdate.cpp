/*
 * OTAUpdate.cpp
 *
 *  Created on: 21 août 2022
 *      Author: julien
 */

#include "OTAUpdate.h"

#if defined(ESP32)
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#elif defined(ESP8266)
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#else
#error fonctionne avec ESP32 ou ESP8266
#endif

AsyncWebServer server(80);

String Texte("ESP OTA");

void DemarrerServeurOTA(const String &pm_Texte)
{
  if (pm_Texte != "")
  {
    Texte = pm_Texte;
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(200, "text/plain", Texte);
  });

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
}

