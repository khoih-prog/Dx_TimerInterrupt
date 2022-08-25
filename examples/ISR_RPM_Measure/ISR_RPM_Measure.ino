/****************************************************************************************************************************
  ISR_RPM_Measure.ino
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

// Important Note: To use drag-and-drop into CURIOSITY virtual drive if you can program via Arduino IDE
// For example, check https://ww1.microchip.com/downloads/en/DeviceDoc/AVR128DB48-Curiosity-Nano-HW-UserG-DS50003037A.pdf

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

#ifdef LED_BUILTIN
  #undef LED_BUILTIN

  // To modify according to your board
  // For Curiosity Nano AVR128DA48 => PIN_PC6
  // For Curiosity Nano AVR128DB48 => PIN_PB3
  #if defined(__AVR_AVR128DA48__) 
    #define LED_BUILTIN   PIN_PC6   // PIN_PB3, 13
  #elif defined(__AVR_AVR128DB48__) 
    #define LED_BUILTIN   PIN_PB3   // PIN_PC6, 13
  #else
    // standard Arduino pin 13
    #define LED_BUILTIN   13
  #endif
#endif

#if defined(__AVR_AVR128DA48__) 
  #define SerialDebug   Serial1
#elif defined(__AVR_AVR128DB48__) 
  #define SerialDebug   Serial3
#else
  // standard Serial
  #define SerialDebug   Serial
#endif

// To modify according to your board
// For Curiosity Nano AVR128DA48 => use SW => PIN_PC7
// For Curiosity Nano AVR128DB48 => use SW => PIN_PB2
#if defined(__AVR_AVR128DA48__) 
  unsigned int interruptPin = PIN_PC7;
#elif defined(__AVR_AVR128DB48__) 
  unsigned int interruptPin = PIN_PB2;
#else
  unsigned int interruptPin = PIN_PA7;      // Original LED_BUILTIN for AVR128DA48/AVR128DB48
#endif

#define TIMER1_INTERVAL_MS        1
#define DEBOUNCING_INTERVAL_MS    80

#define LOCAL_DEBUG      1

volatile unsigned long rotationTime = 0;

float RPM       = 0.00;
float avgRPM    = 0.00;

volatile int debounceCounter;

#define KAVG      100

volatile bool activeState = false;

void detectRotation()
{
  activeState = true;
}

void TimerHandler1()
{
  if ( activeState )
  {
    // Reset to prepare for next round of interrupt
    activeState = false;

    if (debounceCounter >= DEBOUNCING_INTERVAL_MS / TIMER1_INTERVAL_MS )
    {

      //min time between pulses has passed
      RPM = (float) ( 60000.0f / ( rotationTime * TIMER1_INTERVAL_MS ) );

      avgRPM = ( 2 * avgRPM + RPM) / 3,

#if (TIMER_INTERRUPT_DEBUG > 1)
      SerialDebug.print("RPM = "); SerialDebug.print(avgRPM);
      SerialDebug.print(", rotationTime ms = "); SerialDebug.println(rotationTime * TIMER1_INTERVAL_MS);
#endif

      rotationTime = 0;
      debounceCounter = 0;
    }
    else
      debounceCounter++;
  }
  else
  {
    debounceCounter++;
  }

  if (rotationTime >= 5000)
  {
    // If idle, set RPM to 0, don't increase rotationTime
    RPM = 0;

#if (TIMER_INTERRUPT_DEBUG > 1)
    SerialDebug.print("RPM = "); SerialDebug.print(RPM); SerialDebug.print(", rotationTime = "); SerialDebug.println(rotationTime);
#endif

    rotationTime = 0;
  }
  else
  {
    rotationTime++;
  }
}

void setup()
{
  SerialDebug.begin(115200);
  while (!SerialDebug && millis() < 5000);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);

  SerialDebug.print(F("\nStarting ISR_RPM_Measure on ")); SerialDebug.println(BOARD_NAME);
  SerialDebug.println(DX_TIMER_INTERRUPT_VERSION);
  SerialDebug.print(F("CPU Frequency = ")); SerialDebug.print(F_CPU / 1000000); SerialDebug.println(F(" MHz"));

  SerialDebug.print(F("TCB Clock Frequency = ")); 

#if USING_FULL_CLOCK  
  SerialDebug.println(F("Full clock (24/16MHz, etc) for highest accuracy"));
#elif USING_HALF_CLOCK  
  SerialDebug.println(F("Half clock (12/8MHz, etc.) for high accuracy"));
#else
  SerialDebug.println(F("250KHz for lower accuracy but longer time"));
#endif

  // Timer0 is used for micros(), millis(), delay(), etc and can't be used
  // Select Timer 1-2 for UNO, 0-5 for MEGA
  // Timer 2 is 8-bit timer, only for higher frequency

  CurrentTimer.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,

  if (CurrentTimer.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1))
  {
    SerialDebug.print(F("Starting ITimer OK, millis() = "));
    SerialDebug.println(millis());
  }
  else
    SerialDebug.println(F("Can't set ITimer. Select another freq. or timer"));

  // Assumming the interruptPin will go LOW
  attachInterrupt(digitalPinToInterrupt(interruptPin), detectRotation, FALLING);
}

void loop()
{
}
