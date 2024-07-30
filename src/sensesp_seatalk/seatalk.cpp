#include "seatalk.h"

#include <math.h>

#include "sensesp.h"

namespace sensesp {

SeaTalkInput::SeaTalkInput(uint8_t rx_pin) : Startable() {
  rx_pin_ = rx_pin;
}

void SeaTalkInput::start() {
  rx_stream_->begin(4800, SWSERIAL_8S1, rx_pin_, -1, true, 256, 256);
  // enable reading the serial port
  ReactESP::app->onAvailable(*rx_stream_, [this]() {
    while (rx_stream_->available()) {
      seatalk_parser_.handle(rx_stream_->read(), rx_stream_->readParity());
    }
  });
}

} // namespace sensesp
