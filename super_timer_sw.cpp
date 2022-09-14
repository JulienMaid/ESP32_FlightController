/*
 * super_timer_sw.cpp
 *
 *  Created on: 19 janv. 2018
 *      Author: julien
 */

#include "super_timer_sw.h"

Super_TimerEvent_t::Super_TimerEvent_t() :
    TimerEvent_t()
{
  // @TODO Auto-generated constructor stub
  Periode = 0;
  Mini_Periode = 0;
  Nbre_Mini_Periodes = 0;
  Index_Scenario = 0;
  Nouvelle_Config = NULL;
  Super_Periodic = false;

}

Super_TimerEvent_t::~Super_TimerEvent_t()
{
  // @TODO Auto-generated destructor stub
  TimerDelete(this);

  if (Nouvelle_Config != NULL)
  {
    delete Nouvelle_Config;
  }
}

void Super_TimerEvent_t::Init(void (*Callback_Fct)(uint32_t, void*), TimerTime_t Arg_Mini_Periode,
    uint8_t Arg_Nbre_Mini_Periodes, TimerTime_t Arg_Periode, bool Arg_Perodic, void *ArgCallBack, const char Label[])
{
  Mini_Periode = Arg_Mini_Periode;
  Nbre_Mini_Periodes = Arg_Nbre_Mini_Periodes;
  Index_Scenario = Nbre_Mini_Periodes - 1;
  Periode = Arg_Periode;
  Super_Periodic = Arg_Perodic;
  Nouvelle_Config = NULL;
  TimerEvent_t::Init(Callback_Fct, Mini_Periode, Periodic_Timer, ArgCallBack, Label);
}

void Super_TimerEvent_t::Start(void)
{
  TimerEvent_t::Start();
}

void Super_TimerEvent_t::Reload(void)
{
  if (Index_Scenario == 0)
  {
    if (Super_Periodic == Periodic_Timer)
    {
      if (Nouvelle_Config != NULL)
      {
        Periode = Nouvelle_Config->Periode;
        Mini_Periode = Nouvelle_Config->Mini_Periode;
        Nbre_Mini_Periodes = Nouvelle_Config->Nbre_Mini_Periodes;
        delete Nouvelle_Config;
        Nouvelle_Config = NULL;
      }

      Index_Scenario = Nbre_Mini_Periodes - 1;
      Timestamp = Mini_Periode;
    }
    else
    {
      this->Stop();
      this->Delete();
    }
  }
  else if (Index_Scenario == 1)
  {
    Timestamp = Periode;
    Index_Scenario--;

    if (Super_Periodic != Periodic_Timer)
    {
      this->Stop();
      this->Delete();
    }

  }
  else
  {
    Timestamp = Mini_Periode;
    Index_Scenario--;

  }

}

void Super_TimerEvent_t::SetValue(TimerTime_t Arg_Mini_Periode, uint8_t Arg_Nbre_Mini_Periodes, TimerTime_t Arg_Periode)
{
  if (Nouvelle_Config == NULL)
    Nouvelle_Config = new Super_TimerTime_t;

  Nouvelle_Config->Mini_Periode = Arg_Mini_Periode;
  Nouvelle_Config->Nbre_Mini_Periodes = Arg_Nbre_Mini_Periodes;
  Nouvelle_Config->Periode = Arg_Periode;
}
