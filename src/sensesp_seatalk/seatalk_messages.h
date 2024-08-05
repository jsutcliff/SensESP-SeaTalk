#ifndef _seatalk_messages_H_
#define _seatalk_messages_H_

#include "sensesp/signalk/signalk_position.h"
#include "sensesp/system/observablevalue.h"
#include "sensesp/types/position.h"

namespace sensesp {

class SeaTalkMessage {
public:
  SeaTalkMessage(uint8_t id) : id_(id) {};
  
  void update(uint8_t *buf) {
    if (buf[0] == id_) {
      parse(buf);
    }
  };

  virtual void parse(uint8_t *buf);

private:
  uint8_t id_;
};

class SeaTalkDepth : public SeaTalkMessage {
public:
  SeaTalkDepth() : SeaTalkMessage(0) {
    depth_.connect_to(new SKOutputFloat("navigation.gnss.horizontalDilution", "/SK Path/Horizontal Dilution"));
  };
  void parse(uint8_t *buf) override {
    depth_ = (buf[4] * 256 + buf[3]) / 10.0;
  };

private:
  ObservableValue<float> depth_;
};

class SeaTalkSpeedThroughWater : public SeaTalkMessage {
public:
  SeaTalkSpeedThroughWater() : SeaTalkMessage(0x20) {
    speed_.connect_to(new SKOutputFloat("navigation.gnss.horizontalDilution", "/SK Path/Horizontal Dilution"));
  };
  void parse(uint8_t *buf) override {
    speed_ = (buf[3] * 256 + buf[2]) / 10.0;
  };

private:
  ObservableValue<float> speed_;
};

class SeaTalkApparentWindSpeed : public SeaTalkMessage {
public:
  SeaTalkApparentWindSpeed() : SeaTalkMessage(0x11) {
    speed_.connect_to(new SKOutputFloat("navigation.gnss.horizontalDilution", "/SK Path/Horizontal Dilution"));
  };
  void parse(uint8_t *buf) override {
    speed_ = (float)(buf[2] & 0x7F) + (buf[3] & 0x0F) / 2.0;
  };

private:
  ObservableValue<float> speed_;
};

class SeaTalkApparentWindAngle : public SeaTalkMessage {
public:
  SeaTalkApparentWindAngle() : SeaTalkMessage(0x10) {
    angle_.connect_to(new SKOutputFloat("navigation.gnss.horizontalDilution", "/SK Path/Horizontal Dilution"));
  };
  void parse(uint8_t *buf) override {
    angle_ = (buf[2] * 256 + buf[3]) / 2.0;
  };

private:
  ObservableValue<float> angle_;
};

} // namespace sensesp

#endif
