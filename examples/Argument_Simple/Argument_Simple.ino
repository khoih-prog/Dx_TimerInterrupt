/****************************************************************************************************************************
  Argument_Simple.ino
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

unsigned int outputPin1 = LED_BUILTIN;

#define TIMER1_INTERVAL_MS    1000

void TimerHandler1(unsigned int outputPin = LED_BUILTIN)
{
  static bool toggle = false;

#if (TIMER_INTERRUPT_DEBUG > 1)
  //timer interrupt toggles pin outputPin, default LED_BUILTIN
  SerialDebug.print("pin1 = "); SerialDebug.print(outputPin);
  SerialDebug.print(" address: "); SerialDebug.println((uint32_t) &outputPin );
#endif
  
  digitalWrite(outputPin, toggle);
  toggle = !toggle;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  
  SerialDebug.begin(115200);
  while (!SerialDebug && millis() < 5000);

  SerialDebug.print(F("\nStarting Argument_Simple on ")); SerialDebug.println(BOARD_NAME);
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

  // Timer2 is used for micros(), millis(), delay(), etc and can't be used
  CurrentTimer.init();

  if (CurrentTimer.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1, outputPin1))
  {
    SerialDebug.print(F("Starting ITimer OK, millis() = ")); SerialDebug.println(millis());

#if (TIMER_INTERRUPT_DEBUG > 1)    
    SerialDebug.print(F("OutputPin1 = ")); SerialDebug.print(outputPin1);
    SerialDebug.print(F(" address: ")); SerialDebug.println((uint32_t) &outputPin1 );
#endif    
  }
  else
    SerialDebug.println(F("Can't set ITimer. Select another freq. or timer"));
}

void loop()
{
}
