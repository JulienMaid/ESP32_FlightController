#include <Arduino.h>
#include <WiFi.h>
#include <Ticker.h>
#include "timer_sw.h"
#include "super_timer_sw.h"
#include "RTC_soft.h"
#include "trace_debug.h"
#include "outils_wifi.h"
#include "OTAUpdate.h"

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

  SEND_TRACE(INFO, g_t_MsgDemarrage.c_str());

  Send_Trace_Bool(INFO, "Connection Wifi", l_b_WifiConnected);

  if (l_b_WifiConnected == true)
  {
    DemarrerServeurOTA(g_t_MsgDemarrage);
    SEND_TRACE(INFO, "Démarrage Service OTA");
  }

  uint32_t Freq = getCpuFrequencyMhz();
  Serial.print("CPU Freq = ");
  Serial.print(Freq);
  Serial.println(" MHz");
  Freq = getXtalFrequencyMhz();
  Serial.print("XTAL Freq = ");
  Serial.print(Freq);
  Serial.println(" MHz");
  Freq = getApbFrequency();
  Serial.print("APB Freq = ");
  Serial.print(Freq);
  Serial.println(" Hz");
}

// The loop function is called in an endless loop
void loop()
{
  delay(2000);

  SEND_TRACE(INFO, "Test");
}
