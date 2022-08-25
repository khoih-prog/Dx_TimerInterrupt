/****************************************************************************************************************************
  Dx_TimerInterrupt.hpp
  For Arduino AVRDx-based boards (AVR128Dx, AVR64Dx, AVR32Dx, etc.) using DxCore
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/Dx_TimerInterrupt
  Licensed under MIT license

  Now with we can use these new 16 ISR-based timers, while consuming only 1 hwarware Timer.
  Their independently-selected, maximum interval is practically unlimited (limited only by unsigned long miliseconds)
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Version: 1.1.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      23/08/2022 Initial coding to support AVR Dx (AVR128Dx, AVR64Dx, AVR32Dx, etc.) using DxCore
  1.1.0   K.Hoang      24/08/2022 Improve and customize examples for Curiosity Nano AVRDA/AVRDB boards
****************************************************************************************************************************/

#pragma once

#ifndef DX_TIMERINTERRUPT_HPP
#define DX_TIMERINTERRUPT_HPP

//#define TIMER_INTERRUPT_USING_DX_CORE        false

#if defined(DXCORE)

  #define TIMER_INTERRUPT_USING_DX_CORE        true
  
  #if !defined(BOARD_NAME)
  
    ////////////////////////// __AVR_DA__ //////////////////////////
  
    #if ( defined(__AVR_AVR128DA64__) || defined(__AVR_AVR128DA48__) || defined(__AVR_AVR128DA32__) || defined(__AVR_AVR128DA28__) )
      #define BOARD_NAME      F("AVR128DA" )    
    #elif ( defined(__AVR_AVR64DA64__) || defined(__AVR_AVR64DA48__) || defined(__AVR_AVR64DA32__) || defined(__AVR_AVR64DA28__) )
      #define BOARD_NAME      F("AVR64DA")
    #elif ( defined(__AVR_AVR32DA48__) || defined(__AVR_AVR32DA32__) || defined(__AVR_AVR32DA28__) )
      #define BOARD_NAME      F("AVR32DA")
        
    ////////////////////////// __AVR_DB__ //////////////////////////
      
    #elif ( defined(__AVR_AVR128DB64__) || defined(__AVR_AVR128DB48__) || defined(__AVR_AVR128DB32__) || defined(__AVR_AVR128DB28__) )
      #define BOARD_NAME      F("AVR128DB")
    #elif ( defined(__AVR_AVR64DB64__) || defined(__AVR_AVR64DB48__) || defined(__AVR_AVR64DB32__) || defined(__AVR_AVR64DB28__) )
      #define BOARD_NAME      F("AVR64DB")
    #elif ( defined(__AVR_AVR32DB48__) || defined(__AVR_AVR32DB32__) || defined(__AVR_AVR32DB28__) )
      #define BOARD_NAME      F("AVR32DB")
    
    ////////////////////////// __AVR_DD__ //////////////////////////
    
    #elif ( defined(__AVR_AVR64DD32__) || defined(__AVR_AVR64DD28__) || defined(__AVR_AVR64DD20__) || defined(__AVR_AVR64DD14__) )
      #define BOARD_NAME      F("AVR64DD")
      #error AVR64DD not supported yet by the DxCore
    #elif ( defined(__AVR_AVR32DD32__) || defined(__AVR_AVR32DD28__) || defined(__AVR_AVR32DD20__) || defined(__AVR_AVR32DD14__) )
      #define BOARD_NAME      F("AVR32DD")
      #error AVR32DD not supported yet by the DxCore
    #elif ( defined(__AVR_AVR16DD32__) || defined(__AVR_AVR16DD28__) || defined(__AVR_AVR16DD20__) || defined(__AVR_AVR16DD14__) )
      #define BOARD_NAME      F("AVR16DD")
      #error AVR16DD not supported yet by the DxCore
    
    ////////////////////////// __AVR_DU__ //////////////////////////
    
    #elif ( defined(__AVR_AVR64DU32__) || defined(__AVR_AVR64DU28__) )
      #define BOARD_NAME      F("AVR64DU")
    #elif ( defined(__AVR_AVR32DU32__) || defined(__AVR_AVR32DU28__) || defined(__AVR_AVR32DU20__) || defined(__AVR_AVR32DU14__) )
      #define BOARD_NAME      F("AVR32DU")
    #elif ( defined(__AVR_AVR16DU32__) || defined(__AVR_AVR16DU28__) || defined(__AVR_AVR16DU20__) || defined(__AVR_AVR16DU14__) )
      #define BOARD_NAME      F("AVR16DU")
    
    ////////////////////////// __AVR_EA__ //////////////////////////
    
    #elif ( defined(__AVR_AVR64EA48__) || defined(__AVR_AVR64EA32__) || defined(__AVR_AVR64EA28__) )
      #define BOARD_NAME      F("AVR64EA")
    #elif ( defined(__AVR_AVR32EA48__) || defined(__AVR_AVR32EA32__) || defined(__AVR_AVR32EA28__) )
      #define BOARD_NAME      F("AVR32EA")
    #elif ( defined(__AVR_AVR16EA48__) || defined(__AVR_AVR16EA32__) || defined(__AVR_AVR16EA28__) )
      #define BOARD_NAME      F("AVR16EA")
    #elif ( defined(__AVR_AVR8EA48__) || defined(__AVR_AVR8EA32__) || defined(__AVR_AVR8EA28__) )
      #define BOARD_NAME      F("AVR8EA")
      
    ////////////////////////////////////////////////////////////////

    #endif
    
  #endif  // #if !defined(BOARD_NAME)
       
#elif defined(MEGATINYCORE)
      
  #define TIMER_INTERRUPT_USING_MEGATINYCORE       true  
  
  #define BOARD_NAME      F("MEGATINYCORE Board") 
  
  #error Support for megaTinyCore not ready yet! Please check your Tools->Board setting 
      
#else
 
  #error This is designed only for AVRDx boards using DxCore or megaTinyCore ! Please check your Tools->Board setting

#endif

///////////////////////////////////////////////////////////////////////////////

#ifndef TIMER_INTERRUPT_DEBUG
  #define TIMER_INTERRUPT_DEBUG      0
#endif

#include "TimerInterrupt_Generic_Debug.h"

#ifndef DX_TIMER_INTERRUPT_VERSION
  #define DX_TIMER_INTERRUPT_VERSION       			F("Dx_TimerInterrupt v1.1.0")
  
  #define DX_TIMER_INTERRUPT_VERSION_MAJOR      1
  #define DX_TIMER_INTERRUPT_VERSION_MINOR      1
  #define DX_TIMER_INTERRUPT_VERSION_PATCH      0

  #define DX_TIMER_INTERRUPT_VERSION_INT        1001000
#endif

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "pins_arduino.h"

#define MAX_COUNT_16BIT           65535UL

typedef void (*timer_callback)();
typedef void (*timer_callback_p)(void *);

// AVRDx28 and AVRDx32 have 3 TCB timers, (TCB0-TCB2)
// AVRDx48 have 4 TCB timers, (TCB0-TCB3)
// AVRDx64 have 5 TCB timers, (TCB0-TCB4)
// Count only TCB0-TCB3
enum
{
  HW_TIMER_0 = 0,
  HW_TIMER_1,
  HW_TIMER_2,

#if ( ( defined(DX_64_PINS) || defined(DX_48_PINS) ) && defined(TCB3) )
  HW_TIMER_3,
#endif

#if ( defined(DX_64_PINS) && defined(TCB4) )
  HW_TIMER_4,
#endif
  
  NUM_HW_TIMERS
};

#if ( defined(MILLIS_USE_TIMERB0) && USE_TIMER_0)
  #error TCB0 has been used by millis()
#elif ( defined(MILLIS_USE_TIMERB1) && USE_TIMER_1)
  #error TCB1 has been used by millis()  
#elif ( defined(MILLIS_USE_TIMERB2) && USE_TIMER_2)
  #error TCB2 has been used by millis()
#elif ( defined(MILLIS_USE_TIMERB3) && USE_TIMER_3)
  #error TCB3 has been used by millis()
#elif ( defined(MILLIS_USE_TIMERB4) && USE_TIMER_4)
  #error TCB3 has been used by millis()  
#endif

class TimerInterrupt
{
  private:

    bool            _timerDone;
    int8_t          _timer;
    uint32_t        _CCMPValue;
    uint32_t        _CCMPValueRemaining;
    volatile long   _toggle_count;
    double           _frequency;

    void*           _callback;        // pointer to the callback function
    void*           _params;          // function parameter

    void set_CCMP();

  public:

    TimerInterrupt()
    {
      _timer              = -1;
      _frequency          = 0;
      _callback           = NULL;
      _params             = NULL;
      _timerDone          = false;
      _CCMPValue           = 0;
      _CCMPValueRemaining  = 0;
      _toggle_count       = -1;
    };

    explicit TimerInterrupt(const uint8_t& timerNo)
    {
      _timer              = timerNo;
      _frequency          = 0;
      _callback           = NULL;
      _params             = NULL;
      _timerDone          = false;
      _CCMPValue           = 0;
      _CCMPValueRemaining  = 0;
      _toggle_count       = -1;
    };

    void callback() __attribute__((always_inline))
    {
      if (_callback != NULL)
      {
        if (_params != NULL)
          (*(timer_callback_p)_callback)(_params);
        else
          (*(timer_callback)_callback)();
      }
    }

    void init(const int8_t& timer);

    void init()
    {
      init(_timer);
    };

    // frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    bool setFrequency(const float& frequency, timer_callback_p callback, const uint32_t& params, const unsigned long& duration = 0);

    // frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    bool setFrequency(const float& frequency, timer_callback callback, const unsigned long& duration = 0)
    {
      return setFrequency(frequency, reinterpret_cast<timer_callback_p>(callback), /*NULL*/ 0, duration);
    }

    // interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    template<typename TArg>
    bool setInterval(const unsigned long& interval, void (*callback)(TArg), const TArg& params, const unsigned long& duration = 0)
    {
      static_assert(sizeof(TArg) <= sizeof(uint32_t), "setInterval() callback argument size must be <= 4 bytes");
      return setFrequency((float) (1000.0f / interval), reinterpret_cast<timer_callback_p>(callback), (uint32_t) params, duration);
    }

    // interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    bool setInterval(const unsigned long& interval, timer_callback callback, const unsigned long& duration = 0)
    {
      return setFrequency((float) (1000.0f / interval), reinterpret_cast<timer_callback_p>(callback), /*NULL*/ 0, duration);
    }

    template<typename TArg>
    bool attachInterrupt(const float& frequency, void (*callback)(TArg), const TArg& params, const unsigned long& duration = 0)
    {
      static_assert(sizeof(TArg) <= sizeof(uint32_t), "attachInterrupt() callback argument size must be <= 4 bytes");
      return setFrequency(frequency, reinterpret_cast<timer_callback_p>(callback), (uint32_t) params, duration);
    }

    bool attachInterrupt(const float& frequency, timer_callback callback, const unsigned long& duration = 0)
    {
      return setFrequency(frequency, reinterpret_cast<timer_callback_p>(callback), /*NULL*/ 0, duration);
    }

    // Interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    template<typename TArg>
    bool attachInterruptInterval(const unsigned long& interval, void (*callback)(TArg), const TArg& params, const unsigned long& duration = 0)
    {
      static_assert(sizeof(TArg) <= sizeof(uint32_t), "attachInterruptInterval() callback argument size must be <= 4 bytes");
      return setFrequency( (float) ( 1000.0f / interval), reinterpret_cast<timer_callback_p>(callback), (uint32_t) params, duration);
    }

    // Interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    bool attachInterruptInterval(const unsigned long& interval, timer_callback callback, const unsigned long& duration = 0)
    {
      return setFrequency( (float) ( 1000.0f / interval), reinterpret_cast<timer_callback_p> (callback), /*NULL*/ 0, duration);
    }

    void detachInterrupt();

    void disableTimer()
    {
      detachInterrupt();
    }

    // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    void reattachInterrupt(const unsigned long& duration = 0);

    // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    void enableTimer(const unsigned long& duration = 0) __attribute__((always_inline))
    {
      reattachInterrupt(duration);
    }

    // Just stop clock source, still keep the count
    void pauseTimer();

    // Just reconnect clock source, continue from the current count
    void resumeTimer();

    // Just stop clock source, clear the count
    void stopTimer()
    {
      detachInterrupt();
    }

    // Just reconnect clock source, start current count from 0
    void restartTimer(const unsigned long& duration = 0)
    {
      reattachInterrupt(duration);
    }

    int8_t getTimer() __attribute__((always_inline))
    {
      return _timer;
    };

    long getCount() __attribute__((always_inline))
    {
      return _toggle_count;
    };

    void setCount(const long& countInput) __attribute__((always_inline))
    {
      //noInterrupts();

      _toggle_count = countInput;

      //interrupts();
    };

    uint32_t /*long*/ get_CCMPValue() __attribute__((always_inline))
    {
      return _CCMPValue;
    };

    uint32_t /*long*/ get_CCMPValueRemaining() __attribute__((always_inline))
    {
      return _CCMPValueRemaining;
    };

    void adjust_CCMPValue() //__attribute__((always_inline))
    {
      noInterrupts();
      
      if (_CCMPValueRemaining < MAX_COUNT_16BIT)
      {
        set_CCMP();
      }
        
      interrupts();  

      _CCMPValueRemaining -= min(MAX_COUNT_16BIT, _CCMPValueRemaining);

      if (_CCMPValueRemaining <= 0)
      {
        // Reset value for next cycle
        _CCMPValueRemaining = _CCMPValue;
      
        TISR_LOGDEBUG1(F("adjust_CCMPValue: reset _CCMPValueRemaining = "), _CCMPValue);
        _timerDone = true;
      }
      else
        _timerDone = false;
    };

    void reload_CCMPValue() //__attribute__((always_inline))
    {
      noInterrupts();

      // Reset value for next cycle, have to deduct the value already loaded to CCMP register 
      _CCMPValueRemaining = _CCMPValue;
      set_CCMP();
      
      _timerDone = false;

      interrupts();
    };

    bool checkTimerDone() //__attribute__((always_inline))
    {
      return _timerDone;
    };

}; // class TimerInterrupt

//////////////////////////////////////////////

#endif      //#ifndef DX_TIMERINTERRUPT_HPP
