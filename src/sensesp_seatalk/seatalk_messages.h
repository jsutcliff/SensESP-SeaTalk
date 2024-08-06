#ifndef _seatalk_messages_H_
#define _seatalk_messages_H_

#include "sensesp/signalk/signalk_position.h"
#include "sensesp/system/observablevalue.h"
#include "sensesp/types/position.h"

namespace sensesp {

class SeaTalkMessage {
public:
  SeaTalkMessage() {};

  void add_parser(uint8_t id, std::function<void(uint8_t *)> parser) {
    parsers_[id] = parser;
  }

  void parse(uint8_t *buf) {
    for (auto pair : parsers_) {
      if (pair.first == buf[0]) {
        pair.second(buf);
      }
    }
  };

protected:
  std::map<uint8_t, std::function<void(uint8_t *)>> parsers_;
};

class SeaTalkDepth : public SeaTalkMessage {
public:
  SeaTalkDepth() : SeaTalkMessage() {
    SKMetadata *metadata = new SKMetadata();
    metadata->description_ = "Depth Below Transducer";
    metadata->display_name_ = "Depth Below Transducer";
    metadata->short_name_ = "Depth";
    metadata->units_ = "m";

    depth_.connect_to(new SKOutputFloat("environment.depth.belowTransducer", "/SK Path/Depth Below Transducer", metadata));

    add_parser(0x00, [&](uint8_t *buf) -> void {
      depth_ = (buf[4] * 256 + buf[3]) / 10.0 * 0.3048;
    });
  };

private:
  ObservableValue<float> depth_;
};

class SeaTalkSpeedThroughWater : public SeaTalkMessage {
public:
  SeaTalkSpeedThroughWater() : SeaTalkMessage() {
    SKMetadata *metadata = new SKMetadata();
    metadata->description_ = "Speed Through Water";
    metadata->display_name_ = "Speed Through Water";
    metadata->short_name_ = "Speed";
    metadata->units_ = "m/s";

    speed_.connect_to(new SKOutputFloat("navigation.speedThroughWater", "/SK Path/Speed Through Water", metadata));

    add_parser(0x20, [&](uint8_t *buf) -> void {
      speed_ = (buf[3] * 256 + buf[2]) / 10.0 * 1.94384;
    });

    // TODO 0x26
  };

private:
  ObservableValue<float> speed_;
};

class SeaTalkApparentWindSpeed : public SeaTalkMessage {
public:
  SeaTalkApparentWindSpeed() : SeaTalkMessage() {
    SKMetadata *metadata = new SKMetadata();
    metadata->description_ = "Apparent Wind Speed";
    metadata->display_name_ = "Apparent Wind Speed";
    metadata->short_name_ = "Wind Speed";
    metadata->units_ = "m/s";

    speed_.connect_to(new SKOutputFloat("environment.wind.speedApparent", "/SK Path/Apparent Wind Speed", metadata));

    add_parser(0x11, [&](uint8_t *buf) -> void {
      speed_ = (float)(buf[2] & 0x7F) + (buf[3] & 0x0F) / 2.0 * 1.94384;
    });
  };

private:
  ObservableValue<float> speed_;
};

class SeaTalkApparentWindAngle : public SeaTalkMessage {
public:
  SeaTalkApparentWindAngle() : SeaTalkMessage() {
    SKMetadata *metadata = new SKMetadata();
    metadata->description_ = "Apparent wind angle, negative to port";
    metadata->display_name_ = "Apparent Wind Angle";
    metadata->short_name_ = "Wind Angle";
    metadata->units_ = "rad";

    angle_.connect_to(new SKOutputFloat("environment.wind.angleApparent", "/SK Path/Apparent Wind Angle", metadata));

    add_parser(0x10, [&](uint8_t *buf) -> void {
      angle_ = (buf[2] + 256 + buf[3]) / 2.0 * M_PI / 180.0;
    });
  };

private:
  ObservableValue<float> angle_;
};

class SeaTalkWaterTemperature : public SeaTalkMessage {
public:
  SeaTalkWaterTemperature() : SeaTalkMessage() {
    SKMetadata *metadata = new SKMetadata();
    metadata->description_ = "Water Temperature";
    metadata->display_name_ = "Water Temperature";
    metadata->short_name_ = "Water Temp";
    metadata->units_ = "K";

    temp_.connect_to(new SKOutputFloat("environment.water.temperature", "/SK Path/Water Temperature", metadata));

    add_parser(0x23, [&](uint8_t *buf) -> void {
      // temp_ = (float)buf[2]; // TODO units?
      temp_ = ((float)buf[3] - 32.0) * 5.0 / 9.0 + 273.15;
    });

    add_parser(0x27, [&](uint8_t *buf) -> void {
      temp_ = (buf[3] * 256 + buf[2] - 100.0) / 10.0 + 273.15;
    });
  };

private:
  ObservableValue<float> temp_;
};

class SeaTalkPosition : public SeaTalkMessage {
public:
  SeaTalkPosition() : SeaTalkMessage() {
    SKMetadata *metadata = new SKMetadata();
    metadata->description_ = "Position";
    metadata->display_name_ = "Position";
    metadata->short_name_ = "Position";
    metadata->units_ = "deg";

    pos_.connect_to(new SKOutputPosition("navigation.position", "/SK Path/Position", metadata));

    add_parser(0x50, [&](uint8_t *buf) -> void {
      float sign = -1.0 * ((buf[4] * 256 + buf[3]) & 0x8000);
      p_.latitude = sign * (buf[2] + ((buf[4] * 256 + buf[3]) & 0x7FFF) / 5999.88); // TODO check this, set pos/neg
      pos_ = p_;
    });

    add_parser(0x51, [&](uint8_t *buf) -> void {
      float sign = -1.0 * ((buf[4] * 256 + buf[3]) & 0x8000);
      p_.longitude = sign * (buf[2] + ((buf[4] * 256 + buf[3]) & 0x7FFF) / 5999.88); // TODO check this, set pos/neg
      pos_ = p_;
    });
  };

private:
  Position p_; // This is dumb but whatever...
  ObservableValue<Position> pos_;
};

class SeaTalkSpeedOverGround : public SeaTalkMessage {
public:
  SeaTalkSpeedOverGround() : SeaTalkMessage() {
    SKMetadata *metadata = new SKMetadata();
    metadata->description_ = "Speed Over Ground";
    metadata->display_name_ = "Speed Over Ground";
    metadata->short_name_ = "Speed Over Ground";
    metadata->units_ = "m/s";

    speed_.connect_to(new SKOutputFloat("navigation.speedOverGround", "/SK Path/Speed Over Ground", metadata));

    add_parser(0x54, [&](uint8_t *buf) -> void {
      speed_ = (buf[3] * 256 + buf[2]) / 10.0;
    });
  };

private:
  ObservableValue<float> speed_;
};

class SeaTalkAutopilotStuff : public SeaTalkMessage {
public:
  SeaTalkAutopilotStuff() : SeaTalkMessage() {
    heading_.connect_to(new SKOutputFloat("navigation.gnss.horizontalDilution", "/SK Path/Horizontal Dilution"));
    ap_course_.connect_to(new SKOutputFloat("navigation.gnss.horizontalDilution", "/SK Path/Horizontal Dilution"));
    rudder_position_.connect_to(new SKOutputFloat("navigation.gnss.horizontalDilution", "/SK Path/Horizontal Dilution"));

    add_parser(0x84, [&](uint8_t *buf) -> void {
      uint8_t u = buf[4] & 0x0f;
      uint8_t apMode = 0;

      if ((u & 0x02) == 0) {
        apMode = 0;
      } else {
        if ((u & 0x04) == 4) {
          apMode = 2; // steer to wind
        } else {
          if ((u & 0x08) == 8) {
            apMode = 3;
          } else {
            apMode = 1;
          }
        }
      }

      int dir = 0;
      if (apMode > 0) {
        if ((buf[1] & 0x80) == 0x80) {
          dir = 1;
        } else {
          dir = -1;
        }
      }

      float rsa = 0.0;

      if (buf[6] > 127) {
        // negative
        rsa = 0.0 - (float)(256 - buf[6]);
      } else {
        rsa = (float)buf[6];
      }

      u = buf[1] >> 4;
      float hdg = ((u & 0x03) * 90) + (buf[2] & 0x3f) * 2;
      if ((u & 0x4) == 0x4) {
        hdg++;
      }
      if ((hdg < 0) || (hdg > 360)) {
        hdg = 0;
      }

      u = buf[2] >> 6;
      float cts = u * 90 + (buf[3] / 2);

      uint8_t apAlarm = 0;
      if ((buf[5] & 0x04) == 0x04) {
        apAlarm = 1;
      }
      if ((buf[5] & 0x08) == 0x08) {
        apAlarm = 2;
      }
      if ((buf[7] & 0x08) == 0x08) {
        apAlarm = 3;
      }

      heading_ = hdg;
      ap_course_ = cts;
      rudder_position_ = rsa;
    });
  };

private:
  ObservableValue<float> heading_, ap_course_, rudder_position_;
};

} // namespace sensesp

#endif
