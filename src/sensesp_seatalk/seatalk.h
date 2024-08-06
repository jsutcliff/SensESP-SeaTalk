#ifndef _seatalk_H_
#define _seatalk_H_

#include "sensesp/sensors/sensor.h"
#include "sensesp_seatalk/seatalk_parser.h"

#include <SoftwareSerial.h>

namespace sensesp {

/**
 * @brief Support for communicating with SeaTalk
 * instruments over a serial interface.
 *
 * @param rx_pin ESP32 pin for receiving SeaTalk data
 **/
class SeaTalkInput : public Startable {
public:
  SeaTalkInput(uint8_t rx_pin, bool invert=false);
  virtual void start() override final;

private:
  uint8_t rx_pin_;
  bool invert_;
  SoftwareSerial *rx_stream_;
  SeaTalkParser seatalk_parser_;
};

} // namespace sensesp

#endif
