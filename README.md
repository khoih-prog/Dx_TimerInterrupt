## Dx_TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/Dx_TimerInterrupt.svg?)](https://www.ardu-badge.com/Dx_TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/Dx_TimerInterrupt.svg)](https://github.com/khoih-prog/Dx_TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/Dx_TimerInterrupt/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/Dx_TimerInterrupt.svg)](http://github.com/khoih-prog/Dx_TimerInterrupt/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>


---
---

## Table of Contents

* [Why do we need this Dx_TimerInterrupt library](#why-do-we-need-this-Dx_TimerInterrupt-library)
  * [Features](#features)
  * [Why using ISR-based Hardware Timer Interrupt is better](#why-using-isr-based-hardware-timer-interrupt-is-better)
  * [Important Notes about ISR](#important-notes-about-isr)
  * [Currently supported Boards](#currently-supported-boards)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)
* [More useful Information](#more-useful-information)
  * [1. Documents](#1-documents)
  * [2. Timer TCB0-TCB4](#2-timer-tcb0-tcb4)
* [Usage](#usage)
  * [1. Using only Hardware Timer directly](#1-using-only-hardware-timer-directly)
    * [1.1 Init Hardware Timer](#11-init-hardware-timer)
    * [1.2 Set Hardware Timer Interval and attach Timer Interrupt Handler function](#12-set-hardware-timer-interval-and-attach-timer-interrupt-handler-function)
    * [1.3 Set Hardware Timer Frequency and attach Timer Interrupt Handler function](#13-set-hardware-timer-frequency-and-attach-timer-interrupt-handler-function)
  * [2. Using 16 ISR_based Timers from 1 Hardware Timer](#2-using-16-isr_based-timers-from-1-hardware-timer)
    * [2.1 Important Note](#21-important-note)
    * [2.2 Init Hardware Timer and ISR-based Timer](#22-init-hardware-timer-and-isr-based-timer)
    * [2.3 Set Hardware Timer Interval and attach Timer Interrupt Handler functions](#23-set-hardware-timer-interval-and-attach-timer-interrupt-handler-functions)
* [Examples](#examples)
  * [  1. Argument_Complex](examples/Argument_Complex)
  * [  2. Argument_None](examples/Argument_None)
  * [  3. Argument_Simple](examples/Argument_Simple)
  * [  4. Change_Interval](examples/Change_Interval)
  * [  5. FakeAnalogWrite](examples/FakeAnalogWrite)
  * [  6. **ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex).
  * [  7. ISR_RPM_Measure](examples/ISR_RPM_Measure)
  * [  8. **Change_Interval_HF**](examples/Change_Interval_HF)
  * [  9. **ISR_Timers_Array_Simple**](examples/ISR_Timers_Array_Simple).
  * [ 10. RPM_Measure](examples/RPM_Measure)
  * [ 11. SwitchDebounce](examples/SwitchDebounce)
  * [ 12. TimerDuration](examples/TimerDuration)
  * [ 13. TimerInterruptTest](examples/TimerInterruptTest)
  * [ 14. **multiFileProject**](examples/multiFileProject) **New**
* [Example ISR_16_Timers_Array_Complex](#example-isr_16_timers_array_complex)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. ISR_16_Timers_Array_Complex on Arduino AVR128DA](#1-isr_16_timers_array_complex-on-arduino-AVR128DA)
  * [2. ISR_16_Timers_Array_Complex on Arduino AVR128DA to show accuracy difference](#2-isr_16_timers_array_complex-on-arduino-AVR128DA-to-show-accuracy-difference)
    * [2.1 TCB Clock Frequency Full clock for highest accuracy](#21-tcb-clock-frequency-full-clock-for-highest-accuracy)
    * [2.2 TCB Clock Frequency Half clock for high accuracy](#22-tcb-clock-frequency-half-clock-for-high-accuracy)
  * [3. Change_Interval on Arduino AVR128DA](#3-change_interval_HF-on-arduino-AVR128DA)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---


### Why do we need this [Dx_TimerInterrupt library](https://github.com/khoih-prog/Dx_TimerInterrupt)

### Features

This library enables you to use Interrupt from Hardware Timers on Arduino AVRDx-based boards (AVR128Dx, AVR64Dx, AVR32Dx, etc.) using DxCore

As **Hardware Timers are rare, and very precious assets** of any board, this library now enables you to use up to **16 ISR-based Timers, while consuming only 1 Hardware Timer**. Timers' interval is very long (**ulong millisecs**).

Now with these new **16 ISR-based timers**, the maximum interval is **practically unlimited** (limited only by unsigned long miliseconds) while **the accuracy is nearly perfect** compared to software timers. 

The most important feature is they're ISR-based timers. Therefore, their executions are **not blocked by bad-behaving functions / tasks**. This important feature is absolutely necessary for mission-critical tasks. 

The [**ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex) example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.

Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

### Why using ISR-based Hardware Timer Interrupt is better

Imagine you have a system with a **mission-critical function**, measuring water level and control the sump pump or doing something much more important. You normally use a **software timer to poll**, or even place the function in loop(). But what if another function is blocking the loop() or setup().

**So your function might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a **Hardware Timer with Interrupt** to call your function.

**These hardware timers, using interrupt**, still work even if other functions are blocking. Moreover, they are **much more precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules. More to read on:

[**HOWTO Attach Interrupt**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

### Important Notes about ISR

1. Inside the attached function, delay() won’t work and the value returned by millis() will not increment. Serial data received while in the function may be lost. You should declare as volatile any variables that you modify within the attached function.

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.

### Currently supported Boards

- **AVRDA-based boards (AVR128DA, AVR64DA, AVR32DA, etc.) using DxCore**

<p align="center">
    <img src="https://github.com/khoih-prog/Dx_TimerInterrupt/blob/main/pics/Curiosity_AVR128DA48.png">
</p>

- **AVRDB-based boards (AVR128DB, AVR64DB, AVR32DD, etc.) using DxCore**

<p align="center">
    <img src="https://github.com/khoih-prog/Dx_TimerInterrupt/blob/main/pics/Curiosity_AVR128DB48.png">
</p>

### To be supported Boards

- **AVRDD-based boards (AVR64DD) using DxCore**

- **tinyAVR boards using megaTinyCore**

<p align="center">
    <img src="https://github.com/khoih-prog/Dx_TimerInterrupt/blob/main/pics/Curiosity_ATtiny3217.png">
</p>

---
---


## Prerequisites

1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
2. [`SpenceKonde DxCore core 1.4.10+`](https://github.com/SpenceKonde/DxCore) for Arduino AVRDx boards.  [![GitHub release](https://img.shields.io/github/release/SpenceKonde/DxCore.svg)](https://github.com/SpenceKonde/DxCore/releases/latest). Follow [**DxCore Installation**](https://github.com/SpenceKonde/DxCore/blob/master/Installation.md).
3. To use with certain example
   - [`SimpleTimer library`](https://github.com/jfturcot/SimpleTimer) for [ISR_Timers_Array_Simple](examples/ISR_Timers_Array_Simple) and [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex) examples.
   
---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**Dx_TimerInterrupt**](https://github.com/khoih-prog/Dx_TimerInterrupt), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/Dx_TimerInterrupt.svg?)](https://www.ardu-badge.com/Dx_TimerInterrupt) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**Dx_TimerInterrupt**](https://github.com/khoih-prog/Dx_TimerInterrupt) page.
2. Download the latest release `Dx_TimerInterrupt-main.zip`.
3. Extract the zip file to `Dx_TimerInterrupt-main` directory 
4. Copy whole `Dx_TimerInterrupt-main` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO:

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**Dx_TimerInterrupt** library](https://registry.platformio.org/libraries/khoih-prog/Dx_TimerInterrupt) by using [Library Manager](https://registry.platformio.org/libraries/khoih-prog/Dx_TimerInterrupt/installation). Search for Dx_TimerInterrupt in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---

### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using `xyz-Impl.h` instead of standard `xyz.cpp`, possibly creates certain `Multiple Definitions` Linker error in certain use cases.

You can include these `.hpp` files

```
// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "Dx_TimerInterrupt.hpp"   //https://github.com/khoih-prog/Dx_TimerInterrupt

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "Dx_ISR_Timer.hpp"        //https://github.com/khoih-prog/Dx_TimerInterrupt
```

in many files. But be sure to use the following `.h` files **in just 1 `.h`, `.cpp` or `.ino` file**, which must **not be included in any other file**, to avoid `Multiple Definitions` Linker Error

```
// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "Dx_TimerInterrupt.h"     //https://github.com/khoih-prog/Dx_TimerInterrupt

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "Dx_ISR_Timer.h"          //https://github.com/khoih-prog/Dx_TimerInterrupt
```

Check the new [**multiFileProject** example](examples/multiFileProject) for a `HOWTO` demo.

---
---

## More useful Information

### 1. Documents

1. [Arduino 101: Timers and Interrupts](https://www.robotshop.com/community/forum/t/arduino-101-timers-and-interrupts/13072)
2. [Getting Started with Timer/Counter Type B (TCB)](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ApplicationNotes/ApplicationNotes/TB3214-Getting-Started-with-TCB-DS90003214.pdf)
3. [DXCore README.md](https://github.com/SpenceKonde/DxCore/blob/master/README.md)

### 2. Timer TCB0-TCB4

TCB0-TCB are 16-bit timers.

---
---

## Usage

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.

### 1. Using only Hardware Timer directly

### 1.1 Init Hardware Timer

```
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

// Init timer CurrentTimer
CurrentTimer.init();
```

### 1.2 Set Hardware Timer Interval and attach Timer Interrupt Handler function

Use one of these functions with **interval in unsigned long milliseconds**

```
// interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
template<typename TArg> bool setInterval(unsigned long interval, void (*callback)(TArg), TArg params, unsigned long duration = 0);

// interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool setInterval(unsigned long interval, timer_callback callback, unsigned long duration = 0);

// Interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
template<typename TArg> bool attachInterruptInterval(unsigned long interval, void (*callback)(TArg), TArg params, unsigned long duration = 0);

// Interval (in ms) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool attachInterruptInterval(unsigned long interval, timer_callback callback, unsigned long duration = 0)
```

as follows

```
void TimerHandler1()
{
  // Doing something here inside ISR
}

#define TIMER1_INTERVAL_MS        50L

void setup()
{
  ....
  
  // Interval in unsigned long millisecs
    
  // Timer TCB2 is used for micros(), millis(), delay(), etc and can't be used
  ITimer1.init();

  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1))
  {
    Serial.print(F("Starting ITimer OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer. Select another freq. or timer"));  
}  
```

### 1.3 Set Hardware Timer Frequency and attach Timer Interrupt Handler function

Use one of these functions with **frequency in float Hz**

```
// frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool setFrequency(float frequency, timer_callback_p callback, /* void* */ uint32_t params, unsigned long duration = 0);

// frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool setFrequency(float frequency, timer_callback callback, unsigned long duration = 0);

// frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
template<typename TArg> bool attachInterrupt(float frequency, void (*callback)(TArg), TArg params, unsigned long duration = 0);

// frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
bool attachInterrupt(float frequency, timer_callback callback, unsigned long duration = 0);
```

as follows

```
void TimerHandler1()
{
  // Doing something here inside ISR
}

#define TIMER1_FREQ_HZ        5555.555

void setup()
{
  ....
  
  // Frequency in float Hz
  if (ITimer1.attachInterrupt(TIMER1_FREQ_HZ, TimerHandler1))
  {
    Serial.print(F("Starting ITimer OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println("Can't set ITimer. Select another freq. or timer");
}  
```


### 2. Using 16 ISR_based Timers from 1 Hardware Timer

### 2.1 Important Note

The 16 ISR_based Timers, designed for long timer intervals, only support using **unsigned long millisec intervals**. If you have to use much higher frequency or sub-millisecond interval, you have to use the Hardware Timers directly as in [1.3 Set Hardware Timer Frequency and attach Timer Interrupt Handler function](#13-set-hardware-timer-frequency-and-attach-timer-interrupt-handler-function)

### 2.2 Init Hardware Timer and ISR-based Timer

```
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

// Init ISR_Timer
// Each ISR_Timer can service 16 different ISR-based timers
ISR_Timer ISR_Timer1;
```

### 2.3 Set Hardware Timer Interval and attach Timer Interrupt Handler functions

```
void TimerHandler()
{
  ISR_Timer1.run();
}

#define HW_TIMER_INTERVAL_MS          50L

#define TIMER_INTERVAL_2S             2000L
#define TIMER_INTERVAL_5S             5000L
#define TIMER_INTERVAL_11S            11000L
#define TIMER_INTERVAL_101S           101000L

// In AVR, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething2s()
{
  // Doing something here inside ISR every 2 seconds
}
  
void doingSomething5s()
{
  // Doing something here inside ISR every 5 seconds
}

void doingSomething11s()
{
  // Doing something here inside ISR  every 11 seconds
}

void doingSomething101s()
{
  // Doing something here inside ISR every 101 seconds
}

void setup()
{
  ....
  
  // Timer TCB2 is used for micros(), millis(), delay(), etc and can't be used
  CurrentTimer.init();
  
  // Interval in millisecs
  if (CurrentTimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS, TimerHandler))
  {
    lastMillis = millis();
    Serial.print(F("Starting ITimer OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer correctly. Select another freq. or interval"));

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  ISR_Timer1.setInterval(TIMER_INTERVAL_2S, doingSomething2s);
  ISR_Timer1.setInterval(TIMER_INTERVAL_5S, doingSomething5s);
  ISR_Timer1.setInterval(TIMER_INTERVAL_11S, doingSomething11s);
  ISR_Timer1.setInterval(TIMER_INTERVAL_101S, doingSomething101s);
}  
```

---
---

### Examples: 

 1. [Argument_Complex](examples/Argument_Complex)
 2. [Argument_None](examples/Argument_None)
 3. [Argument_Simple](examples/Argument_Simple)
 4. [Change_Interval](examples/Change_Interval).
 5. [FakeAnalogWrite](examples/FakeAnalogWrite).
 6. [**ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex).
 7. [ISR_RPM_Measure](examples/ISR_RPM_Measure)
 8. [**Change_Interval_HF**](examples/Change_Interval_HF)
 9. [**ISR_Timers_Array_Simple**](examples/ISR_Timers_Array_Simple).
10. [RPM_Measure](examples/RPM_Measure)
11. [SwitchDebounce](examples/SwitchDebounce)
12. [TimerDuration](examples/TimerDuration)
13. [TimerInterruptTest](examples/TimerInterruptTest)
14. [**multiFileProject**](examples/multiFileProject)

---

### Example [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex)

https://github.com/khoih-prog/Dx_TimerInterrupt/blob/0441816fc4540a8f7c03247950ad4386defda202/examples/ISR_16_Timers_Array_Complex/ISR_16_Timers_Array_Complex.ino#L16-L369


---
---

### Debug Terminal Output Samples

### 1. ISR_16_Timers_Array_Complex on Arduino AVR128DA

The following is the sample terminal output when running example [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex) on **Arduino AVR128DA** to demonstrate the accuracy of ISR Hardware Timer, **especially when system is very busy**.  The ISR timer is **programmed for 2s, is activated exactly after 2.000s !!!**

While software timer, **programmed for 2s, is activated after more than 10.000s in loop().

```
Starting ISR_16_Timers_Array_Complex on AVR128DA
Dx_TimerInterrupt v1.0.0
CPU Frequency = 16 MHz
TCB Clock Frequency = 16MHz for highest accuracy
Starting  ITimer OK, millis() = 6
SimpleTimer : 2, ms : 10006, Dms : 10006
Timer : 0, programmed : 5000, actual : 5006
Timer : 1, programmed : 10000, actual : 10010
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 20065, Dms : 10059
Timer : 0, programmed : 5000, actual : 4994
Timer : 1, programmed : 10000, actual : 9998
Timer : 2, programmed : 15000, actual : 15014
Timer : 3, programmed : 20000, actual : 20008
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 30125, Dms : 10060
Timer : 0, programmed : 5000, actual : 4994
Timer : 1, programmed : 10000, actual : 9998
Timer : 2, programmed : 15000, actual : 14992
Timer : 3, programmed : 20000, actual : 20008
Timer : 4, programmed : 25000, actual : 25012
Timer : 5, programmed : 30000, actual : 30006
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 40184, Dms : 10059
Timer : 0, programmed : 5000, actual : 5004
Timer : 1, programmed : 10000, actual : 10008
Timer : 2, programmed : 15000, actual : 14992
Timer : 3, programmed : 20000, actual : 20006
Timer : 4, programmed : 25000, actual : 25012
Timer : 5, programmed : 30000, actual : 30006
Timer : 6, programmed : 35000, actual : 35010
Timer : 7, programmed : 40000, actual : 40014
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 50245, Dms : 10061
Timer : 0, programmed : 5000, actual : 5004
Timer : 1, programmed : 10000, actual : 9998
Timer : 2, programmed : 15000, actual : 15002
Timer : 3, programmed : 20000, actual : 20006
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30006
Timer : 6, programmed : 35000, actual : 35010
Timer : 7, programmed : 40000, actual : 40014
Timer : 8, programmed : 45000, actual : 45008
Timer : 9, programmed : 50000, actual : 50012
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 60307, Dms : 10062
Timer : 0, programmed : 5000, actual : 5004
Timer : 1, programmed : 10000, actual : 9998
Timer : 2, programmed : 15000, actual : 15002
Timer : 3, programmed : 20000, actual : 19996
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30004
Timer : 6, programmed : 35000, actual : 35010
Timer : 7, programmed : 40000, actual : 40014
Timer : 8, programmed : 45000, actual : 45008
Timer : 9, programmed : 50000, actual : 50012
Timer : 10, programmed : 55000, actual : 55006
Timer : 11, programmed : 60000, actual : 60010
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 70369, Dms : 10062
Timer : 0, programmed : 5000, actual : 4994
Timer : 1, programmed : 10000, actual : 9998
Timer : 2, programmed : 15000, actual : 15002
Timer : 3, programmed : 20000, actual : 19996
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30004
Timer : 6, programmed : 35000, actual : 34998
Timer : 7, programmed : 40000, actual : 40014
Timer : 8, programmed : 45000, actual : 45008
Timer : 9, programmed : 50000, actual : 50012
Timer : 10, programmed : 55000, actual : 55006
Timer : 11, programmed : 60000, actual : 60010
Timer : 12, programmed : 65000, actual : 65014
Timer : 13, programmed : 70000, actual : 70008
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 80432, Dms : 10063
Timer : 0, programmed : 5000, actual : 4994
Timer : 1, programmed : 10000, actual : 9998
Timer : 2, programmed : 15000, actual : 15002
Timer : 3, programmed : 20000, actual : 19996
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30004
Timer : 6, programmed : 35000, actual : 34998
Timer : 7, programmed : 40000, actual : 39992
Timer : 8, programmed : 45000, actual : 45008
Timer : 9, programmed : 50000, actual : 50012
Timer : 10, programmed : 55000, actual : 55006
Timer : 11, programmed : 60000, actual : 60010
Timer : 12, programmed : 65000, actual : 65014
Timer : 13, programmed : 70000, actual : 70008
Timer : 14, programmed : 75000, actual : 75012
Timer : 15, programmed : 80000, actual : 80016
```

---


### 2. ISR_16_Timers_Array_Complex on Arduino AVR128DA to show accuracy difference


### 2.1. TCB Clock Frequency Full clock for highest accuracy


```
Starting ISR_16_Timers_Array_Complex on AVR128DA
Dx_TimerInterrupt v1.0.0
CPU Frequency = 16 MHz
TCB Clock Frequency = 16MHz for highest accuracy
Starting  ITimer OK, millis() = 6
SimpleTimer : 2, ms : 10007, Dms : 10007
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10006
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 20066, Dms : 10059
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15006
Timer : 3, programmed : 20000, actual : 20006
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0

...


SimpleTimer : 2, ms : 211269, Dms : 10064
Timer : 0, programmed : 5000, actual : 5000            <========== Very accurate @ clock 16MHz
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40000
Timer : 8, programmed : 45000, actual : 45000
Timer : 9, programmed : 50000, actual : 50000
Timer : 10, programmed : 55000, actual : 55000
Timer : 11, programmed : 60000, actual : 60000
Timer : 12, programmed : 65000, actual : 65000
Timer : 13, programmed : 70000, actual : 70000
Timer : 14, programmed : 75000, actual : 75000
Timer : 15, programmed : 80000, actual : 80000
SimpleTimer : 2, ms : 221333, Dms : 10064
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40000
Timer : 8, programmed : 45000, actual : 45000
Timer : 9, programmed : 50000, actual : 50000
Timer : 10, programmed : 55000, actual : 55000
Timer : 11, programmed : 60000, actual : 60000
Timer : 12, programmed : 65000, actual : 65000
Timer : 13, programmed : 70000, actual : 70000
Timer : 14, programmed : 75000, actual : 75000
Timer : 15, programmed : 80000, actual : 80000

```

---

### 2.2. TCB Clock Frequency Half clock for high accuracy

```

Starting ISR_16_Timers_Array_Complex on AVR128DA
Dx_TimerInterrupt v1.0.0
CPU Frequency = 16 MHz
TCB Clock Frequency = Half clock (12/8MHz, etc.) for high accuracy
Starting  ITimer OK, millis() = 10
SimpleTimer : 2, ms : 10011, Dms : 10011
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10011
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0

...


SimpleTimer : 2, ms : 160949, Dms : 10064
Timer : 0, programmed : 5000, actual : 5000            <========== Very accurate @ clock 8MHz
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40000
Timer : 8, programmed : 45000, actual : 45000
Timer : 9, programmed : 50000, actual : 50000
Timer : 10, programmed : 55000, actual : 55000
Timer : 11, programmed : 60000, actual : 60000
Timer : 12, programmed : 65000, actual : 65000
Timer : 13, programmed : 70000, actual : 70000
Timer : 14, programmed : 75000, actual : 75000
Timer : 15, programmed : 80000, actual : 80000
SimpleTimer : 2, ms : 171013, Dms : 10064
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40000
Timer : 8, programmed : 45000, actual : 45000
Timer : 9, programmed : 50000, actual : 50000
Timer : 10, programmed : 55000, actual : 55000
Timer : 11, programmed : 60000, actual : 60000
Timer : 12, programmed : 65000, actual : 65000
Timer : 13, programmed : 70000, actual : 70000
Timer : 14, programmed : 75000, actual : 75000
Timer : 15, programmed : 80000, actual : 80000
```

---

### 3. Change_Interval_HF on Arduino AVR128DA

The following is the sample terminal output when running example [Change_Interval_HF](examples/Change_Interval_HF) on **Arduino AVR128DA** to demonstrate how to change High Frequency Timer Interval on-the-fly

```
Starting Change_Interval_HF on AVR128DA
Dx_TimerInterrupt v1.0.0
CPU Frequency = 16 MHz
TCB Clock Frequency = 16MHz for highest accuracy
[TISR] TCB 1
[TISR] ==================
[TISR] Init, Timer = 1
[TISR] CTRLB   = 0
[TISR] CCMP    = 65535
[TISR] INTCTRL = 0
[TISR] CTRLA   = 1
[TISR] ==================
[TISR] Frequency = 10000.00 , CLK_TCB_FREQ = 16000000
[TISR] setFrequency: _CCMPValueRemaining =  1600
Starting ITimer OK, millis() = 12
Frequency, Timer = 10000
Time = 10001, Timer1Count = 99856
Time = 20002, Timer1Count = 199807
[TISR] Frequency = 5000.00 , CLK_TCB_FREQ = 16000000
[TISR] setFrequency: _CCMPValueRemaining =  3200
Changing Frequency, Timer = 5000
Time = 30003, Timer1Count = 249792
Time = 40004, Timer1Count = 299784
[TISR] Frequency = 10000.00 , CLK_TCB_FREQ = 16000000
[TISR] setFrequency: _CCMPValueRemaining =  1600
Changing Frequency, Timer = 10000
```

---
---


### Debug

Debug is enabled by default on Serial.

You can also change the debugging level from 0 to 3

```cpp
// These define's must be placed at the beginning before #include "Dx_TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---

### Issues

Submit issues to: [Dx_TimerInterrupt issues](https://github.com/khoih-prog/Dx_TimerInterrupt/issues)

---
---

### TO DO

1. Search for bug and improvement
2. Add support to **AVRDD-based boards (AVR64DD)** using [DxCore](https://github.com/SpenceKonde/DxCore)
3. Add support to **tinyAVRDD-based boards** using [megaTinyCore](https://github.com/SpenceKonde/megaTinyCore)
4. Add support to 250KHz Timer Frequency


### DONE

 1. Longer Interval for timers
 2. Reduce code size if use less timers. Eliminate compiler warnings
 3. Now supporting complex object pointer-type argument
 4. 16 hardware-initiated software-enabled timers while using only 1 hardware timer
 5. Add support to **AVRDx-based boards (AVR128Dx, AVR64Dx, AVR32Dx, etc.) using DxCore**
 6. Selectable **TCB Clock FULL, HALF** depending on necessary accuracy
 6. Fix `multiple-definitions` linker error
 7. Optimize library code by using `reference-passing` instead of `value-passing`



---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library. Especially to these people who have directly or indirectly contributed to this [Dx_TimerInterrupt library](https://github.com/khoih-prog/Dx_TimerInterrupt)

1. Thanks to good work of [Spence Konde (aka Dr. Azzy)](https://github.com/SpenceKonde) for the good [DxCore](https://github.com/SpenceKonde/DxCore) and [megaTinyCore](https://github.com/SpenceKonde/megaTinyCore)
2. Thanks to [LaurentR59](https://github.com/LaurentR59) to request the enhancement [Support for DX CORE CPU and MightyCORE CPU possible? #8](https://github.com/khoih-prog/TimerInterrupt_Generic/issues/8) leading to this new library

<table>
  <tr>
    <td align="center"><a href="https://github.com/SpenceKonde"><img src="https://github.com/SpenceKonde.png" width="100px;" alt="SpenceKonde"/><br/><sub><b>⭐️⭐️ Spence Konde</b></sub></a><br/></td>
    <td align="center"><a href="https://github.com/LaurentR59"><img src="https://github.com/LaurentR59.png" width="100px;" alt="LaurentR59"/><br/><sub><b>LaurentR59</b></sub></a><br/></td>
  </tr> 
</table>

---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

## License

- The library is licensed under [MIT](https://github.com/khoih-prog/Dx_TimerInterrupt/blob/main/LICENSE)

---

## Copyright

Copyright 2022- Khoi Hoang


