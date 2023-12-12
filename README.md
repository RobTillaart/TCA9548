
[![Arduino CI](https://github.com/RobTillaart/TCA9548/workflows/Arduino%20CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![Arduino-lint](https://github.com/RobTillaart/TCA9548/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/RobTillaart/TCA9548/actions/workflows/arduino-lint.yml)
[![JSON check](https://github.com/RobTillaart/TCA9548/actions/workflows/jsoncheck.yml/badge.svg)](https://github.com/RobTillaart/TCA9548/actions/workflows/jsoncheck.yml)
[![GitHub issues](https://img.shields.io/github/issues/RobTillaart/TCA9548.svg)](https://github.com/RobTillaart/TCA9548/issues)

[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/RobTillaart/TCA9548/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/release/RobTillaart/TCA9548.svg?maxAge=3600)](https://github.com/RobTillaart/TCA9548/releases)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/robtillaart/library/TCA9548.svg)](https://registry.platformio.org/libraries/robtillaart/TCA9548)


# TCA9548

Arduino Library for TCA9548 I2C multiplexer and compatibles.


## Description

Library for the TCA9548 and TCA9548a (PCA9548, PCA9548a, PCA9546, PCA9545, PCA9543) I2C multiplexer.

The library allows you to enable 0 to 7 I2C channels (ports) uniquely or simultaneously.
In fact the TCA9548 is therefore a **switch**, although often named a multiplexer.
A multiplexer is especially useful if you have multiple identical devices that have a fixed address,
a too small address range or if there are address conflicts between different I2C devices.

**Warning**
The library is not tested extensively.

The library caches the channels enabled, and if a channel is enabled,
it will not be enabled again (low level) to optimize performance.

#### I2C 

I2C address of the device itself is 0x70 .. 0x77.
This address can not be used on any of the I2C channels of course.

Note if your first multiplexer is 0x70, you may have an array of 0x71 multiplexers behind it.
(giving you 8 x 8 = 64 I2C buses, a lot of admin overhead and probably performance penalties).

Pull-up resistors are REQUIRED on all upstream and downstream channels.


#### 0.2.0 Breaking change

Version 0.2.0 introduced a breaking change.
You cannot set the pins in **begin()** any more.
This reduces the dependency of processor dependent Wire implementations.
The user has to call **Wire.begin()** and can optionally set the Wire pins 
before calling **begin()**.


#### Compatible devices

This library is expected to work for the following devices: (since 0.2.1)

|  Device    |  Tested  |  Notes  |
|:-----------|:--------:|:-------:|
|  TCA9548s  |    n     |
|  PCA9548   |    n     |  see links below  |
|  PCA9548a  |    n     |
|  PCA9546   |    n     |  see links below  |
|  PCA9545   |    n     |  see links below  |
|  PCA9543   |    n     |  see links below  |

Note: not tested with hardware yet.

There are however small differences, check the data sheets to see the details.
- [difference TCA PCA](https://e2e.ti.com/support/interface-group/interface/f/interface-forum/815758/faq-what-is-the-difference-between-an-i2c-device-with-the-family-name-pca-and-tca)
- https://electronics.stackexchange.com/questions/209616/is-nxps-pca9548a-compatible-with-tis-tca9548a
- https://www.nxp.com/docs/en/application-note/AN262.pdf


#### Related

- https://github.com/RobTillaart/HC4051  (1x8 mux)
- https://github.com/RobTillaart/HC4052  (2x4 mux)
- https://github.com/RobTillaart/HC4053  (3x2 mux)
- https://github.com/RobTillaart/HC4067  (1x16 mux)


## Interface

```cpp
#include "TCA9548.h"
```

#### Constructor

- **TCA9548(const uint8_t deviceAddress, TwoWire \*wire = &Wire)** Constructor.
deviceAddress = 0x70 .. 0x77, wire = Wire or WireN.
- **bool begin(uint8_t dataPin, uint8_t clockPin, uint8_t mask = 0x00)**  Set I2C pins for ESP32.
Set mask of channels to be enabled, default all disabled.
- **bool begin(uint8_t mask = 0x00)**  set mask of channels to be enabled, default all disabled.
- **bool isConnected()** returns true if address of the multiplexer is found on I2C bus.


The derived classes PCA9548/PCA9546 have the same interface, except constructor.
(see #15)

- **PCA9548(const uint8_t deviceAddress, TwoWire \*wire = &Wire)** Constructor.
deviceAddress = 0x70 .. 0x77, wire = Wire or WireN.
- **PCA9546(const uint8_t deviceAddress, TwoWire \*wire = &Wire)** Constructor.
deviceAddress = 0x70 .. 0x77, wire = Wire or WireN.
- **PCA9545(const uint8_t deviceAddress, TwoWire \*wire = &Wire)** Constructor.
deviceAddress = 0x70 .. 0x77, wire = Wire or WireN.
- **PCA9543(const uint8_t deviceAddress, TwoWire \*wire = &Wire)** Constructor.
deviceAddress = 0x70 .. 0x77, wire = Wire or WireN.


#### Find device

- **bool isConnected(uint8_t address)** returns true if arbitrary address is found on I2C bus.
This can be used to verify a certain device is available (or not) on an enabled channel.


#### Channel functions

All "channel functions" return true on success.

- **bool enableChannel(uint8_t channel)** enables channel 0 .. 7 non-exclusive.
Multiple channels can be enabled in parallel.
- **bool disableChannel(uint8_t channel)** disables channel 0 .. 7.
Will not disable other channels.
- **bool selectChannel(uint8_t channel)** enables a single channel 0 .. 7 exclusive.
All other channels will be disabled in the same call, so not before or after.
- **bool isEnabled(uint8_t channel)** returns true if the channel is enabled.
- **bool disableAllChannels()** fast way to disable all.
- **bool setChannelMask(uint8_t mask)** enables 0 or more channels simultaneously with a bit mask.
- **uint8_t getChannelMask()** reads back the bit mask of the channels enabled.


#### Reset

- **void setResetPin(uint8_t resetPin)** sets the pin to reset the chip. (Not tested)
- **void reset()** trigger the reset pin.
- **int getError()** returns the last I2C error.


#### Forced IO

When forced IO is set all writes and read - **getChannelMask()** - will go to the device.
If the flag is set to false it will cache the value of the channels enabled.
This will result in more responsive / faster calls.
Note that writes are only optimized if the channels are already set.

- **void setForced(bool forced = false)** set forced write, slower but more robust.
  - forced == false == fast mode (default).
  - forced == true == slower, robust mode.
- **bool getForced()** returns set flag.


#### Interrupts

The PCA9545 and PCA9543 do support interrupts. 
These two derived classes have implemented

- **uint8_t getInterruptMask()** function that returns a bit mask of interrupts set.


## Error Codes

Not implemented yet, preparation for future.

|  name                   |  value  |  description            |
|:------------------------|:-------:|:------------------------|
|  TCA9548_OK             |   00    |  no error               |
|  TCA9548_ERROR_I2C      |  -10    |  detected an I2C error  |
|  TCA9548_ERROR_CHANNEL  |  -20    |  channel out of range   |


## Future


#### Must

- improve documentation.
- improve error handling.

#### Should

- add examples.
- test test and test.
- write unit test.


#### Could

- set an "always enabled" mask.
  - investigate the consequences!

#### Wont


## Support

If you appreciate my libraries, you can support the development and maintenance.
Improve the quality of the libraries by providing issues and Pull Requests, or
donate through PayPal or GitHub sponsors.

Thank you,



