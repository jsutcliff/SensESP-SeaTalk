#ifndef _seatalk_parser_H_
#define _seatalk_parser_H_

#include <map>

#include "sensesp/signalk/signalk_position.h"
#include "sensesp/system/observablevalue.h"
#include "sensesp/types/position.h"

#define MAX_BUF_SIZE 20

namespace sensesp {

/**
 * @brief NMEA 0183 parser class.
 *
 * Data from an input stream is fed to the parse one character at a time.
 *
 * At initialization, the parser registers a set of sentence parsers.
 * When input data matches a sentence, the parser calls the appropriate
 * sentence parser.
 */
class SeaTalkParser {
public:
  SeaTalkParser();
  void handle(char c, bool parity);

private:
  bool bCmd = false;    // set true if command found
  uint8_t bufCount = 0; // counter for no of bytes received
  uint8_t cmdCount;     // number of bytes in command
  int i;
  uint16_t b;
  UBaseType_t uB;
  uint8_t stBuff[MAX_BUF_SIZE];
  uint32_t mS;
  char b1[15];
  uint8_t u;
  uint16_t aveAWA[5] = {0, 0, 0, 0, 0};
  uint8_t iCount = 0;
  uint8_t apMode; // autopilot mode
  uint8_t old_apMode = 0;
  uint8_t cmd;
  uint16_t ui16;
  bool working = false;
  uint16_t lastAWA;
  int wts; // wind angle to steer
  int dir; // direction to steer
  float rsa, stw, sog, xte, aws, dpt, dtw, vlw;
  uint16_t hdg, cts, cog, awa, btw; // heading, course to steer, course over ground, apparent wind angle, bearing to waypt
  uint8_t apAlarm;
  bool xteValid = false, btwValid = false;       // cross track error valid
};

} // namespace sensesp

#endif
