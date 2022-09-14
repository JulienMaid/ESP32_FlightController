#include "timer_sw.h"
#include <stdint.h>
#include <string.h>

//#ifdef ARDUINO
//#include "timer_sw_arduino.h"
//#define TimerHwStart()  MsTimer2::start()
//#define TimerHwStop()   MsTimer2::stop()
//#else
#define TimerHwStart()  {}
#define TimerHwStop()   {}
//#endif

bool TimerHwStarted;

TimerEvent_t *TimerEvent_t::TimerListHead = NULL;
bool TimerEvent_t::IRQ_Timer_SW_En_Cours = false;

TimerEvent_t::TimerEvent_t(void (*Callback_Fct)(uint32_t, void*), TimerTime_t remainingTime, bool Perodic,
    void *ArgCallBack, const char Label[])
{
  Init(Callback_Fct, remainingTime, Perodic, ArgCallBack, Label);
}

TimerEvent_t::TimerEvent_t(void)
{
  Init(NULL, 0, false, NULL);
}

TimerEvent_t::TimerEvent_t(TimerEvent_t &Timer)
{
  memcpy(this, &Timer, sizeof(TimerEvent_t));

  if (TimerExists(&Timer) == true)
  {
    this->InsertTimer();
  }

}

TimerEvent_t& TimerEvent_t::operator=(TimerEvent_t const &Timer)
{
  memcpy(this, &Timer, sizeof(TimerEvent_t));

  if (TimerExists(&Timer) == true)
  {
    this->InsertTimer();
  }

  return *this;
}

bool TimerEvent_t::operator==(TimerEvent_t const &Timer1)
{
  if (ReloadValue != Timer1.ReloadValue)
    return false;

  if (Callback != (void*) &Timer1.Callback)
    return false;

  if (Periodic != Timer1.Periodic)
    return false;

  return true;
}

TimerEvent_t::~TimerEvent_t(void)
{
  TimerDelete(this);
}

void TimerEvent_t::Start()
{
  if (TimerHwStarted == true)
    TimerHwStop();

  Reload();
  Top = false;
  IsRunning = true;

  // si le timer n'est pas dans la liste on l'y insere
  if (TimerExists(this) == false)
  {
#if UTILISER_TRACE_DEBUG_DS_TIMER_SW == 1
    Send_Trace_IfNotZero(DBGX, "Erreur TimerInsterTimer", TimerInsertTimer(obj), true);
#else // UTILISER_TRACE_DEBUG_DS_TIMER_SW == 1
    InsertTimer();
#endif // UTILISER_TRACE_DEBUG_DS_TIMER_SW == 1
  }

  TimerHwStart();

#if UTILISER_TRACE_DEBUG_DS_TIMER_SW == 1
  Send_Timer_Trace(obj->Trace_Level, obj, TIMER_START, true);
#endif // UTILISER_TRACE_DEBUG_DS_TIMER_SW == 1

}

void TimerEvent_t::Stop()
{
  IsRunning = false;
}

void TimerEvent_t::Init(void (*Callback_Fct)(uint32_t, void*), TimerTime_t remainingTime, bool Periodic,
    void *ArgCallBack, const char Label[])
{
  if (TimerExists(this))
  {
    if (!TimerDelete(this))
    {
      Init(Callback_Fct, remainingTime, Periodic, ArgCallBack, Label);
      return;
    }
  }

  this->Callback = Callback_Fct;
  this->ArgCallBack = ArgCallBack;
  this->Timestamp = remainingTime;
  this->ReloadValue = remainingTime;
  this->Periodic = Periodic;
  this->Label = Label;
  this->Registered = false;
  this->u32_userValue = 0;
}

void TimerEvent_t::SetValue(TimerTime_t Time)
{
  //Timestamp = Time;
  ReloadValue = Time;
}

void TimerEvent_t::Reload(void)
{
  Timestamp = ReloadValue;
}

TimerTime_t TimerEvent_t::Time_Elapsed(void)
{
  return ReloadValue - Timestamp;
}

TimerTime_t TimerEvent_t::Time_Remaining(void)
{
  return Timestamp;
}

bool TimerEvent_t::DoLaunchMe(void)
{
  return Top;
}

bool TimerEvent_t::Exists(void)
{
  return TimerExists(this);
}

bool TimerEvent_t::Delete(void)
{
  return TimerDelete(this);
}

bool TimerEvent_t::IsTop(void)
{
  if (Top == true)
  {
#if UTILISER_TRACE_DEBUG_DS_TIMER_SW == 1
    Send_Timer_Trace(obj->Trace_Level, obj, TIMER_TOP, true);
#endif ///UTILISER_TRACE_DEBUG_DS_TIMER_SW == 1
    Top = false;
    return true;
  }

  if ((Registered == false) && (IsRunning == true))
  {
#if UTILISER_TRACE_DEBUG_DS_TIMER_SW == 1
    uint8_t Temp[60] = "Erreur TimerIsTop: Timer ";
    strcat(Temp, obj->Label);
    strcat(Temp, " non charge");
    Send_Trace(DBGX, Temp, true);
#endif // UTILISER_TRACE_DEBUG_DS_TIMER_SW == 1
  }

  if (IRQ_Timer_SW_En_Cours == true) // pour eviter de rester bloquer en boucle infinie...
    return true;

  return false;
}

bool TimerEvent_t::TimerClean(void)
{
  TimerEvent_t *cur = TimerListHead;
  bool State_TimerHw_Bak;
  bool Timer_List_Cleaned = false;

  if (TimerListHead == NULL)
    return false;

  State_TimerHw_Bak = TimerHwStarted;

  if (TimerHwStarted == true)
    TimerHwStop();

  do
  {
    if ((cur->IsRunning == false) && (cur->Registered == true))
    {
#if UTILISER_TRACE_DEBUG_DS_TIMER_SW == 1
      Send_Trace_IfNotZero(DBGX, "Erreur TimerDelete depuis TimerClean", TimerDelete(cur), true);
#else // UTILISER_TRACE_DEBUG_DS_TIMER_SW == 1
      TimerDelete(cur);
#endif // UTILISER_TRACE_DEBUG_DS_TIMER_SW == 1
      Timer_List_Cleaned = true;
    }

    cur = cur->Next;

    if (cur == NULL) // Pour eviter d'executer la condition (cur->Next != NULL) avec cur == NULL...
      break;
  }
  while (cur->Next != NULL);

  if (State_TimerHw_Bak == true)
    TimerHwStart();

  return Timer_List_Cleaned;
}

bool TimerEvent_t::TimerExists(const TimerEvent_t *obj)
{
  TimerEvent_t *cur = TimerListHead;
  bool State_TimerHw_Bak;

  State_TimerHw_Bak = TimerHwStarted;

  if (TimerHwStarted == true)
    TimerHwStop();

  while (cur != NULL)
  {
    if (cur == obj)
    {
      if (State_TimerHw_Bak == true)
        TimerHwStart();

      return true;
    }
    cur = cur->Next;
  }

  if (State_TimerHw_Bak == true)
    TimerHwStart();

  return false;
}

uint8_t TimerEvent_t::TimerDelete(TimerEvent_t *obj)
{
  bool State_TimerHw_Bak;

  State_TimerHw_Bak = TimerHwStarted;

  if (TimerHwStarted == true)
  {
    TimerHwStop();
  }

  if (TimerExists(obj) == false)
  {
    if (State_TimerHw_Bak == true)
    {
      TimerHwStart();
    }

    return 0;
  }

  TimerEvent_t *cur = TimerListHead;
  TimerEvent_t *prec = NULL;

  if (cur == NULL)
  {
    if (State_TimerHw_Bak == true)
    {
      TimerHwStart();
    }

    return 0;
  }

  do
  {
    if (cur == obj) // timer trouve, on le saute dans la liste chainee
    {
      obj->IsRunning = false; // Timer marque comme desactive

      if (prec != NULL)
        prec->Next = cur->Next;
      else
        TimerListHead = cur->Next;

      obj->Registered = false;

      if (State_TimerHw_Bak == true)
      {
        TimerHwStart();
      }

#if UTILISER_TRACE_DEBUG_DS_TIMER_SW == 1
      if(obj->Trace_Level != NONE)
        Send_Timer_Trace(obj->Trace_Level + 1, obj, TIMER_DELETE, true);
      else
        Send_Timer_Trace(obj->Trace_Level, obj, TIMER_DELETE, true);
#endif // UTILISER_TRACE_DEBUG_DS_TIMER_SW == 1

      return 0;
    }

    prec = cur;
    cur = cur->Next;

  }
  while (cur != NULL);

  if (State_TimerHw_Bak == true)
  {
    TimerHwStart();
  }
  // Si on arrive ici c'est que le timer n'a pas ete trouve
  return 1;

}

uint8_t TimerEvent_t::InsertTimer(void)
{
  if (TimerExists(this) == true)
    return 1;

  Next = TimerListHead;

  TimerListHead = this;

  Registered = true;

  return 0;
}

void TimerEvent_t::setUserValeur(uint32_t value)
{
  this->u32_userValue = value;
}

uint32_t TimerEvent_t::getUserValue(void)
{
  return this->u32_userValue;
}

void Inc_Timer(void)
{

  bool Timers_Stopped = true;

  // Si pas de timer enregistre on sort !
  if (TimerEvent_t::TimerListHead == NULL)
  {
    TimerHwStop();
    return;
  }

  TimerEvent_t *P_Timer_Courant = TimerEvent_t::TimerListHead;

  do
  {
    if (P_Timer_Courant->IsRunning == true)
    {
      Timers_Stopped = false;

      if (P_Timer_Courant->Timestamp == 0)
      {
        //Arret du timer en cours
        P_Timer_Courant->IsRunning = false;
      }
      else
      {
        if (P_Timer_Courant->Timestamp < DEC_TIMESTAMP)
          P_Timer_Courant->Timestamp = 0;
        else
          P_Timer_Courant->Timestamp -= DEC_TIMESTAMP;

        if (P_Timer_Courant->Timestamp == 0)
        {
          if (P_Timer_Courant->Callback != NULL)
          {
#if UTILISER_TRACE_DEBUG_DS_TIMER_SW == 1
            Send_Timer_Trace(P_Timer_Courant->Trace_Level, P_Timer_Courant, TIMER_TOP, true);
#endif // UTILISER_TRACE_DEBUG_DS_TIMER_SW == 1
            P_Timer_Courant->CallBack_Launched = true;
            TimerEvent_t::IRQ_Timer_SW_En_Cours = true;
            P_Timer_Courant->Callback(P_Timer_Courant->u32_userValue, P_Timer_Courant->ArgCallBack);
            TimerEvent_t::IRQ_Timer_SW_En_Cours = false;
            P_Timer_Courant->CallBack_Launched = false;
          }
          else
            P_Timer_Courant->Top = true;

          if (P_Timer_Courant->Periodic == true)
            P_Timer_Courant->Reload();
          else
          {
            // Si le timer a ete relance dans sa fonction Callback, ne pas l'effacer !
            if (P_Timer_Courant->Timestamp == 0)
            {
              TimerEvent_t::TimerDelete(P_Timer_Courant);
            }

          }
        }
      }
    }

    P_Timer_Courant = P_Timer_Courant->Next;
  }
  while (P_Timer_Courant != NULL);

  TimerEvent_t::TimerClean();

  if (Timers_Stopped == true) // ts les timers SW sont sur stop, plus besoin du timer HW
  {
    TimerHwStop();
  }

}

