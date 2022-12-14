/****************************************************************************************************************************
  Dx_ISR_Timer.hpp
  For Arduino AVRDx-based boards (AVR128Dx, AVR64Dx, AVR32Dx, etc.) using DxCore
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/Dx_TimerInterrupt
  Licensed under MIT license

  Now with we can use these new 16 ISR-based timers, while consuming only 1 hwarware Timer.
  Their independently-selected, maximum interval is practically unlimited (limited only by unsigned long miliseconds)
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Version: 1.1.3

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      23/08/2022 Initial coding to support AVR Dx (AVR128Dx, AVR64Dx, AVR32Dx, etc.) using DxCore
  1.1.0   K.Hoang      24/08/2022 Improve and customize examples for Curiosity Nano AVRDA/AVRDB boards
  1.1.1   K.Hoang      24/08/2022 Using Serial1 instead of Serial for debugging with Curiosity Nano AVRDA/AVRDB
  1.1.2   K.Hoang      24/08/2022 Using Serial3 for debugging with Curiosity Nano AVRDB
  1.1.3   K.Hoang      27/08/2022 Fix TCB0 disable bug
*****************************************************************************************************************************/

#pragma once

#ifndef DX_ISR_TIMER_HPP
#define DX_ISR_TIMER_HPP

#if ( defined(DXCORE) || defined(MEGATINYCORE) )

#else
  #error This is designed only for AVRDx boards using DxCore or megaTinyCore ! Please check your Tools->Board setting
#endif

#ifndef DX_TIMER_INTERRUPT_VERSION
  #define DX_TIMER_INTERRUPT_VERSION       			F("Dx_TimerInterrupt v1.1.2")
  
  #define DX_TIMER_INTERRUPT_VERSION_MAJOR      1
  #define DX_TIMER_INTERRUPT_VERSION_MINOR      1
  #define DX_TIMER_INTERRUPT_VERSION_PATCH      2

  #define DX_TIMER_INTERRUPT_VERSION_INT        1001002
#endif

#include "TimerInterrupt_Generic_Debug.h"

#include <stddef.h>
#include <inttypes.h>

#if defined(ARDUINO)
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#endif

typedef void (*timer_callback)();
typedef void (*timer_callback_p)(void *);

class ISR_Timer 
{
  public:
    // maximum number of timers
    const static int MAX_TIMERS = 16;

    // setTimer() constants
    const static int RUN_FOREVER = 0;
    const static int RUN_ONCE = 1;

    // constructor
    ISR_Timer();

    void  init();

    // this function must be called inside loop()
    void  run();

    // Timer will call function 'f' every 'd' milliseconds forever
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setInterval(const unsigned long& d, timer_callback f);

    // Timer will call function 'f' with parameter 'p' every 'd' milliseconds forever
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setInterval(const unsigned long& d, timer_callback_p f, void* p);

    // Timer will call function 'f' after 'd' milliseconds one time
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setTimeout(const unsigned long& d, timer_callback f);

    // Timer will call function 'f' with parameter 'p' after 'd' milliseconds one time
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setTimeout(const unsigned long& d, timer_callback_p f, void* p);

    // Timer will call function 'f' every 'd' milliseconds 'n' times
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setTimer(const unsigned long& d, timer_callback f, const unsigned& n);

    // Timer will call function 'f' with parameter 'p' every 'd' milliseconds 'n' times
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setTimer(const unsigned long& d, timer_callback_p f, void* p, const unsigned& n);

    // updates interval of the specified timer
    bool changeInterval(const unsigned& numTimer, const unsigned long& d);

    // destroy the specified timer
    void deleteTimer(const unsigned& numTimer);

    // restart the specified timer
    void restartTimer(const unsigned& numTimer);

    // returns true if the specified timer is enabled
    bool isEnabled(const unsigned& numTimer);

    // enables the specified timer
    void enable(const unsigned& numTimer);

    // disables the specified timer
    void disable(const unsigned& numTimer);

    // enables all timers
    void enableAll();

    // disables all timers
    void disableAll();

    // enables the specified timer if it's currently disabled,
    // and vice-versa
    void toggle(const unsigned& numTimer);

    // returns the number of used timers
    unsigned  getNumTimers();

    // returns the number of available timers
    unsigned  getNumAvailableTimers() 
    {
      return MAX_TIMERS - numTimers;
    };

  private:
  
    // deferred call constants
    const static int DEFCALL_DONTRUN    = 0;    // don't call the callback function
    const static int DEFCALL_RUNONLY    = 1;    // call the callback function but don't delete the timer
    const static int DEFCALL_RUNANDDEL  = 2;    // call the callback function and delete the timer

    // low level function to initialize and enable a new timer
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int  setupTimer(const unsigned long& d, void* f, void* p, bool h, const unsigned& n);

    // find the first available slot
    int  findFirstFreeSlot();

    typedef struct 
    {
      unsigned long prev_millis;        // value returned by the millis() function in the previous run() call
      void* callback;                   // pointer to the callback function
      void* param;                      // function parameter
      bool hasParam;                 // true if callback takes a parameter
      unsigned long delay;              // delay value
      unsigned maxNumRuns;              // number of runs to be executed
      unsigned numRuns;                 // number of executed runs
      bool enabled;                  // true if enabled
      unsigned toBeCalled;              // deferred function call (sort of) - N.B.: only used in run()
    } timer_t;

    volatile timer_t timer[MAX_TIMERS];

    // actual number of timers in use (-1 means uninitialized)
    volatile int numTimers;
};

#endif  // DX_ISR_TIMER_HPP
