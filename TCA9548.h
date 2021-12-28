#pragma once
//
//    FILE: TCA9548.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.2
//    DATE: 2021-03-16
// PURPOSE: Library for TCA9548 I2C multiplexer
//
//     URL: https://github.com/RobTillaart/TCA9548
//


#include "Arduino.h"
#include "Wire.h"


#define TCA9548_LIB_VERSION             (F("0.1.2"))


class TCA9548
{
public:
  // address = 0x70 .. 0x77
  TCA9548(const uint8_t deviceAddress, TwoWire *wire = &Wire);

#if defined (ESP8266) || defined(ESP32)
  bool    begin(uint8_t sda, uint8_t scl, uint8_t mask = 0x00);  // default no channels enabled
#endif
  bool    begin(uint8_t mask = 0x00);         // default no channels enabled
  bool    isConnected();                      // find multiplexer on I2C bus
  bool    isConnected(uint8_t address);       // find any address on I2C bus

  // channel = 0.. 7
  void    enableChannel(uint8_t channel);
  void    disableChannel(uint8_t channel);
  void    selectChannel(uint8_t channel);   // enable only this channel
  bool    isEnabled(uint8_t channel);

  // mask = 0x00 .. 0xFF - every bit is a channel.
  void    setChannelMask(uint8_t mask);
  uint8_t getChannelMask();

  void    setResetPin(uint8_t resetPin);
  void    reset();     // trigger reset pin

  // set forced IO (default false)
  void    setForced(bool forced = false) { _forced = forced; };
  bool    getForced() { return _forced; };

  int     getError();

private:
  uint8_t   _mask;             // caching mask
  uint8_t   _resetPin;
  int       _error;
  uint8_t   _address;
  TwoWire*  _wire;
  bool      _forced;
};


// -- END OF FILE --

