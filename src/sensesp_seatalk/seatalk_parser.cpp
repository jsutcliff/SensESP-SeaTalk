#include "seatalk_parser.h"
#include "sensesp.h"

namespace sensesp {

SeaTalkParser::SeaTalkParser() {
  add_msg(new SeaTalkDepth());
  add_msg(new SeaTalkSpeedThroughWater());
  add_msg(new SeaTalkApparentWindSpeed());
  add_msg(new SeaTalkApparentWindAngle());
  add_msg(new SeaTalkWaterTemperature());
  add_msg(new SeaTalkPosition());
  add_msg(new SeaTalkSpeedOverGround());
  add_msg(new SeaTalkAutopilotStuff());
}

void SeaTalkParser::add_msg(SeaTalkMessage *msg) {
  msgs_.push_back(msg);
}

void SeaTalkParser::handle(uint8_t b, bool parity) {
  if (!parity) {
    buf[bufCount++] = b;

    // this byte gives the number of bytes in cmd
    if (bufCount == 2) {
      cmdCount = (b & 0x000F) + 3; // cmd + byte count + mandatory 1st field
      debugD("Parsed msg length for: %04x = %i", buf[0], cmdCount);
    }

    if ((cmdCount == bufCount) && (cmdCount > 2)) {
      debugD("Message complete, looking for a parser for: %04x", buf[0]);

      for (auto msg : msgs_) {
        msg->parse(buf);
      }

      bufCount = 0;
      cmdCount = 0;
    }
  }
}

} // namespace sensesp
