/****************************************************************************************************************************
  Argument_None.ino
  For Arduino AVRDx-based boards (AVR128Dx, AVR64Dx, AVR32Dx, etc.) using DxCore
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/Dx_TimerInterrupt
  Licensed under MIT license

  Now with we can use these new 16 ISR-based timers, while consuming only 1 hwarware Timer.
  Their independently-selected, maximum interval is practically unlimited (limited only by unsigned long miliseconds)
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.
 *****************************************************************************************************************************/

#if !( defined(DXCORE) || defined(MEGATINYCORE) )
  #error This is designed only for DXCORE or MEGATINYCORE megaAVR board! Please check your Tools->Board setting
#endif

// These define's must be placed at the beginning before #include "megaAVR_TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

// Select USING_FULL_CLOCK      == true for  24/16MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_HALF_CLOCK      == true for  12/ 8MHz to Timer TCBx => shorter timer, but better accuracy
// Select USING_250KHZ          == true for 250KHz to Timer TCBx => longer timer,  but worse  accuracy
// Not select for default 250KHz to Timer TCBx => longer timer,  but worse accuracy
#define USING_FULL_CLOCK      true
#define USING_HALF_CLOCK      false
#define USING_250KHZ          false         // Not supported now

#define USE_TIMER_0           false
#define USE_TIMER_1           true
#define USE_TIMER_2           false         // Normally used by millis(). Don't use
#define USE_TIMER_3           false
#define USE_TIMER_4           false

#if USE_TIMER_0
  #define CurrentTimer   ITimer0
#elif USE_TIMER_1
  #define CurrentTimer   ITimer1
#elif USE_TIMER_2
  #define CurrentTimer   ITimer2
#elif USE_TIMER_3
  #define CurrentTimer   ITimer3
#elif USE_TIMER_4
  #define CurrentTimer   ITimer4
#else
  #error You must select one Timer  
#endif

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "Dx_TimerInterrupt.h"

#define TIMER1_INTERVAL_MS    1000

#ifndef LED_BUILTIN
  #define LED_BUILTIN   13
#endif

void TimerHandler1(void)
{
  static bool toggle1 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(LED_BUILTIN, OUTPUT);
  }

  //timer interrupt toggles pin LED_BUILTIN
  digitalWrite(LED_BUILTIN, toggle1);
  toggle1 = !toggle1;
}

void setup()
{
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  Serial.print(F("\nStarting Argument_None on ")); Serial.println(BOARD_NAME);
  Serial.println(DX_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  Serial.print(F("TCB Clock Frequency = ")); 

#if USING_FULL_CLOCK  
  Serial.println(F("Full clock (24/16MHz, etc) for highest accuracy"));
#elif USING_HALF_CLOCK  
  Serial.println(F("Half clock (12/8MHz, etc.) for high accuracy"));
#else
  Serial.println(F("250KHz for lower accuracy but longer time"));
#endif

  // Timer TCB2 is used for micros(), millis(), delay(), etc and can't be used
  CurrentTimer.init();

  if (CurrentTimer.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1))
  {
    Serial.print(F("Starting ITimer OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer. Select another freq. or timer"));
}

void loop()
{
}
