#ifndef _seatalk_parser_H_
#define _seatalk_parser_H_

#include <vector>

#include "seatalk_messages.h"
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
  void add_msg(SeaTalkMessage *msg);
  void handle(char c, bool parity);

private:
  std::vector<SeaTalkMessage *> msgs_;

  bool bCmd = false;    // set true if command found
  uint8_t bufCount = 0; // counter for no of bytes received
  uint8_t cmdCount;     // number of bytes in command
  uint16_t b;
  uint8_t stBuff[MAX_BUF_SIZE];
  char b1[15];
  bool working = false;
};

} // namespace sensesp

#endif
