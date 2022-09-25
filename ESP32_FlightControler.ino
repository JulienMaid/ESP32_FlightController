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

Ticker g_t_blinker;
String g_t_MsgDemarrage("ESP32 Flight controller");

//The setup function is called once at startup of the sketch
void setup()
{
  bool l_b_WifiConnected = false;

  Serial.begin(115200);
  Serial.println();

  g_t_blinker.attach(0.05, Inc_Timer);

  Init_RTC_Soft();
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
}

// The loop function is called in an endless loop
void loop()
{
  uint8_t var = 8;
  delay(2000);

  SEND_VTRACE(INFO, "Test2");
  Serial.println(xPortGetCoreID());

}
