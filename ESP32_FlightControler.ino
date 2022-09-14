#include <Arduino.h>
#include <WiFi.h>
#include <Ticker.h>
#include "timer_sw.h"
#include "super_timer_sw.h"
#include "RTC_soft.h"
#include "trace_debug.h"
#include "outils_wifi.h"
#include "OTAUpdate.h"

Ticker blinker;

//The setup function is called once at startup of the sketch
void setup()
{
  bool l_b_WifiConnected = false;

  Serial.begin(115200);
  Serial.println();

  blinker.attach(0.05, Inc_Timer);

  Init_RTC_Soft();
  Set_Max_Debug_Level(DBG1);

  l_b_WifiConnected = connecterWifi(0);

  Send_Trace_Bool(INFO, "Connection Wifi", l_b_WifiConnected);

  if (l_b_WifiConnected == true)
  {
    DemarrerServeurOTA("ESP32 Flight controller");
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
