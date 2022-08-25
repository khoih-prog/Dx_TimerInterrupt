/****************************************************************************************************************************
  multiFileProject.ino
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

#include "multiFileProject.h"

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "Dx_TimerInterrupt.h"

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "Dx_ISR_Timer.h"

void setup() 
{
  // put your setup code here, to run once:
}

void loop() 
{
  // put your main code here, to run repeatedly:
}
