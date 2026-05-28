// Add after MakerbaseFOC class in makerbase_foc.h
#include "esphome/components/fan/fan.h"

class MakerbaseFOCFan : public Component, public fan::Fan {
 public:
  void set_parent(MakerbaseFOC *parent) { parent_ = parent; }
  void set_motor_id(int id) { motor_id_ = id; }

  void setup() override { this->state = false; this->speed = 0; this->publish_state(); }

  fan::FanTraits get_traits() override {
    auto t = fan::FanTraits();
    t.set_speed(true);
    t.set_supported_speed_count(100);
    t.set_direction(true);
    return t;
  }

  void control(const fan::FanCall &call) override {
    if (call.get_state().has_value()) {
      this->state = *call.get_state();
      if (this->state) parent_->enable_motor(motor_id_);
      else parent_->disable_motor(motor_id_);
    }
    if (call.get_speed().has_value()) this->speed = *call.get_speed();
    if (call.get_direction().has_value()) this->direction = *call.get_direction();

    if (this->state) {
      float spd = (this->speed / 100.0f) * 15.0f;
      if (this->direction == fan::FanDirection::REVERSE) spd = -spd;
      parent_->set_speed(motor_id_, spd);
    }
    this->publish_state();
  }

 protected:
  MakerbaseFOC *parent_{nullptr};
  int motor_id_{1};
};