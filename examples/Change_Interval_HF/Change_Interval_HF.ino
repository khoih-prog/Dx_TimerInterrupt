/****************************************************************************************************************************
  Change_Interval_HF.ino
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

#define TIMER1_FREQUENCY            50UL

volatile uint32_t Timer1Count = 0;

void printResult(uint32_t currTime)
{
  Serial1.print(F("Time = ")); Serial1.print(currTime);
  Serial1.print(F(", Timer1Count = ")); Serial1.println(Timer1Count);
}

void TimerHandler1()
{
  static bool toggle = false;
  
  Timer1Count++;

  //timer interrupt toggles pin LED_BUILTIN
  digitalWrite(LED_BUILTIN, toggle);
  toggle = !toggle;
}

void setup()
{
  Serial1.begin(115200);
  while (!Serial1 && millis() < 5000);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial1.print(F("\nStarting Change_Interval_HF on ")); Serial1.println(BOARD_NAME);
  Serial1.println(DX_TIMER_INTERRUPT_VERSION);
  Serial1.print(F("CPU Frequency = ")); Serial1.print(F_CPU / 1000000); Serial1.println(F(" MHz"));
  
  Serial1.print(F("TCB Clock Frequency = ")); 

#if USING_FULL_CLOCK  
  Serial1.println(F("Full clock (24/16MHz, etc) for highest accuracy"));
#elif USING_HALF_CLOCK  
  Serial1.println(F("Half clock (12/8MHz, etc.) for high accuracy"));
#else
  Serial1.println(F("250KHz for lower accuracy but longer time"));
#endif

  // Timer2 is used for micros(), millis(), delay(), etc and can't be used
  CurrentTimer.init();

  if (CurrentTimer.attachInterrupt(TIMER1_FREQUENCY, TimerHandler1))
  {
    Serial1.print(F("Starting ITimer OK, millis() = ")); Serial1.println(millis());
    Serial1.print(F("Frequency, Timer = ")); Serial1.println(TIMER1_FREQUENCY);
  }
  else
    Serial1.println(F("Can't set ITimer. Select another freq. or timer"));
}

#define CHECK_INTERVAL_MS     1000L
#define CHANGE_INTERVAL_MS    5000L

void loop()
{
  static uint32_t lastTime = 0;
  static uint32_t lastChangeTime = 0;
  static uint32_t currTime;
  static uint32_t multFactor = 0;

  currTime = millis();

  if (currTime - lastTime > CHECK_INTERVAL_MS)
  {
    printResult(currTime);
    lastTime = currTime;

    if (currTime - lastChangeTime > CHANGE_INTERVAL_MS)
    {
      //setInterval(unsigned long interval, timerCallback callback)
      multFactor = (multFactor + 1) % 50;

      // interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
      // bool setInterval(unsigned long interval, timer_callback callback, unsigned long duration)

      CurrentTimer.setFrequency(TIMER1_FREQUENCY / (multFactor + 1), TimerHandler1);

      Serial1.print(F("Changing Frequency, Timer = ")); Serial1.println(TIMER1_FREQUENCY / (multFactor + 1));

      lastChangeTime = currTime;
    }
  }
}
