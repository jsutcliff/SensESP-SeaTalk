/**
 * @file seatalk_receiver.cpp
 * @brief SeaTalk 1 receiver example.
 *
 */

#include "sensesp_app_builder.h"
#include "sensesp_seatalk/seatalk.h"

using namespace sensesp;

ReactESP app;

void setup() {
  SensESPAppBuilder builder;
  sensesp_app = builder.set_hostname("sensesp-seatalk")->get_app();

#ifndef SERIAL_DEBUG_DISABLED
  SetupSerialDebug(115200);
#endif

  SeaTalkInput *seatalk_input = new SeaTalkInput(1);

  sensesp_app->start();
}

void loop() { app.tick(); }
