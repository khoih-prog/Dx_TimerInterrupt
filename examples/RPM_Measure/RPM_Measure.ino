/************************************************
  RPM_Measure.ino
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
/* RPM Measuring uses high frequency hardware timer 1Hz == 1ms) to measure the time from of one rotation, in ms
   then convert to RPM. One rotation is detected by reading the state of a magnetic REED SW or IR LED Sensor
   Asssuming LOW is active.
   For example: Max speed is 600RPM => 10 RPS => minimum 100ms a rotation. We'll use 80ms for debouncing
   If the time between active state is less than 8ms => consider noise.
   RPM = 60000 / (rotation time in ms)

   You can also use interrupt to detect whenever the SW is active, set a flag
   then use timer to count the time between active state
*/

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

// To modify according to your board
// For Curiosity Nano AVR128DA48 => use SW => PIN_PC7
// For Curiosity Nano AVR128DB48 => use SW => PIN_PB2
#if defined(__AVR_AVR128DA48__) 
  unsigned int SWPin = PIN_PC7;
#elif defined(__AVR_AVR128DB48__) 
  unsigned int SWPin = PIN_PB2;
#else
  unsigned int SWPin = A0;
#endif

#define TIMER1_INTERVAL_MS        1
#define DEBOUNCING_INTERVAL_MS    80

#define LOCAL_DEBUG               1

volatile unsigned long rotationTime = 0;
float RPM       = 0.00;
float avgRPM    = 0.00;

volatile int debounceCounter;

#define KAVG      100

void TimerHandler1()
{
  if ( !digitalRead(SWPin) && (debounceCounter >= DEBOUNCING_INTERVAL_MS / TIMER1_INTERVAL_MS ) )
  {
    //min time between pulses has passed
    RPM = (float) ( 60000.0f / ( rotationTime * TIMER1_INTERVAL_MS ) );

    avgRPM = ( 2 * avgRPM + RPM) / 3,

#if (TIMER_INTERRUPT_DEBUG > 1)
      Serial.print("RPM = "); Serial.print(avgRPM);
      Serial.print(", rotationTime ms = "); Serial.println(rotationTime * TIMER1_INTERVAL_MS);
#endif

    rotationTime = 0;
    debounceCounter = 0;
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
    Serial.print("RPM = "); Serial.print(RPM); Serial.print(", rotationTime = "); Serial.println(rotationTime);
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
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  pinMode(SWPin, INPUT_PULLUP);

  Serial.print(F("\nStarting RPM_Measure on ")); Serial.println(BOARD_NAME);
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

  // Timer2 is used for micros(), millis(), delay(), etc and can't be used
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
