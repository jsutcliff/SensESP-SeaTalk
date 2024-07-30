#ifndef _seatalk_H_
#define _seatalk_H_

#include "sensesp/sensors/sensor.h"
#include "sensesp_seatalk/seatalk_parser.h"

#include <SoftwareSerial.h>

namespace sensesp {

/**
 * @brief Support for a GPS module communicating with NMEA 0183
 * messages over a serial interface.
 *
 * @param rx_stream Pointer to the Stream of incoming GPS data over
 * a serial connection.
 **/
class SeaTalkInput : public Startable {
public:
  SeaTalkInput(uint8_t rx_pin);
  virtual void start() override final;

private:
  uint8_t rx_pin_;
  SoftwareSerial *rx_stream_;
  SeaTalkParser seatalk_parser_;
};

} // namespace sensesp

#endif
