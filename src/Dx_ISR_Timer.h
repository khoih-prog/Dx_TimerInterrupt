/****************************************************************************************************************************
  Dx_ISR_Timer.h
  For Arduino AVRDx-based boards (AVR128Dx, AVR64Dx, AVR32Dx, etc.) using DxCore
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/Dx_TimerInterrupt
  Licensed under MIT license

  Now with we can use these new 16 ISR-based timers, while consuming only 1 hwarware Timer.
  Their independently-selected, maximum interval is practically unlimited (limited only by unsigned long miliseconds)
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Version: 1.1.1

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K.Hoang      23/08/2022 Initial coding to support AVR Dx (AVR128Dx, AVR64Dx, AVR32Dx, etc.) using DxCore
  1.1.0   K.Hoang      24/08/2022 Improve and customize examples for Curiosity Nano AVRDA/AVRDB boards
  1.1.1   K.Hoang      24/08/2022 Using Serial1 instead of Serial for debugging with Curiosity Nano AVRDA/AVRDB
*****************************************************************************************************************************/

#pragma once

#ifndef DX_ISR_TIMER_H
#define DX_ISR_TIMER_H

#include "Dx_ISR_Timer.hpp"
#include "Dx_ISR_Timer_Impl.h"

#endif  // DX_ISR_TIMER_H
