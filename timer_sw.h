#pragma once

#include <stdint.h>
#include <stdlib.h>

enum Timer_Type
{
  Periodic_Timer = true, One_Shot_Timer = false
};

#ifndef TimerTime_t
typedef uint64_t TimerTime_t;
#endif

#define DEC_TIMESTAMP 50

class TimerEvent_t
{
public:
  TimerEvent_t(void (*Callback_Fct)(uint32_t, void*), TimerTime_t remainingTime,
      bool Perodic = true, void *ArgCallBack = NULL, const char Label[] = NULL);
  TimerEvent_t(void);
  TimerEvent_t(TimerEvent_t&);
  virtual ~TimerEvent_t(void);

  TimerEvent_t& operator=(TimerEvent_t const&);
  bool operator==(TimerEvent_t const&);

  virtual void Init(void (*Callback_Fct)(uint32_t, void*), TimerTime_t remainingTime, bool Perodic =
      true, void *ArgCallBack = NULL, const char Label[] = NULL);
  virtual void Start(void);
  virtual void Stop(void);
  virtual void SetValue(TimerTime_t Time);
  virtual void Reload(void);

  bool Exists(void);
  bool Delete(void);
  bool DoLaunchMe(void);
  bool IsTop(void);

  TimerTime_t Time_Remaining(void);
  TimerTime_t Time_Elapsed(void);

  void setUserValeur(uint32_t value);
  uint32_t getUserValue(void);
  friend void Inc_Timer(void);

protected:
  static TimerEvent_t *TimerListHead;
  static bool TimerExists(const TimerEvent_t *obj);
  static uint8_t TimerDelete(TimerEvent_t *obj);
  static bool TimerClean(void);

  static bool IRQ_Timer_SW_En_Cours;

  uint32_t u32_userValue;
  TimerTime_t Timestamp;         //! Current timer value
  TimerTime_t ReloadValue;       //! Timer delay value
  bool IsRunning;             //! Is the timer currently running
  bool Periodic;				//! Is the timer periodic
  bool CallBack_Launched;
  bool Top;
  bool Registered;
  void (*Callback)(uint32_t value, void *ArgCallBack); //! Timer IRQ callback function
  void *ArgCallBack;
  const char *Label;
  class TimerEvent_t *Next;  //! Pointer to the next Timer object.

  unsigned char InsertTimer(void);

};

void Inc_Timer(void);
