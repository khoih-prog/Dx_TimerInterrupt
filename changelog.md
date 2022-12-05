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

* [Changelog](#changelog)
  * [Release v1.1.3](#release-v113)
  * [Release v1.1.2](#release-v112)
  * [Release v1.1.1](#release-v111)
  * [Release v1.1.0](#release-v110)
  * [Initial Release v1.0.0](#initial-release-v100)

---
---

## Changelog

### Release v1.1.3

1. Fix bug giving error when using TCB0 (`USE_TIMER_0 == true`)
2. Fix bug causing system crash when using `_TIMERINTERRUPT_LOGLEVEL_ > 0`

### Release v1.1.2

1. Using `Serial3` for debugging with `Curiosity Nano AVRDB`, and `Serial1` for debugging with `Curiosity Nano AVRDA`


### Release v1.1.1

1. Using Serial1 instead of Serial for debugging with Curiosity Nano AVRDA/AVRDB
2. Update `README.md`

### Release v1.1.0

1. Improve and customize examples for `Curiosity Nano AVRDA/AVRDB` boards to use on-board LED and SW
2. Add notes `howto upload by drag-and-drop` to `CURIOSITY` virtual drive

### Initial Release v1.0.0

1. Initial release to support Arduino **AVRDx-based boards (AVR128Dx, AVR64Dx, AVR32Dx, etc.) using DxCore**



