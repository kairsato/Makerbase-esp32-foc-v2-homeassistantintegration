#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/fan/fan.h"
#include <string>

namespace esphome {
namespace makerbase_foc {

class MakerbaseFOC : public Component, public uart::UARTDevice {
 public:
  void setup() override {}
  void loop() override {}

  void send_command(const std::string &cmd) {
    this->write_str(cmd.c_str());
    this->write_byte('\n');
  }

  void set_motor_speed(int motor, float speed) {
    char buf[32];
    snprintf(buf, sizeof(buf), "M%d %.2f", motor, speed);
    this->send_command(std::string(buf));
  }

  void enable_motor(int motor) {
    char buf[16];
    snprintf(buf, sizeof(buf), "M%d E1", motor);
    this->send_command(std::string(buf));
  }

  void disable_motor(int motor) {
    char buf[16];
    snprintf(buf, sizeof(buf), "M%d E0", motor);
    this->send_command(std::string(buf));
  }
};

class MakerbaseFOCFan : public Component, public fan::Fan {
 public:
  void set_parent(MakerbaseFOC *parent) { this->parent_ = parent; }
  void set_motor_id(int motor_id) { this->motor_id_ = motor_id; }

  void setup() override {
    this->state = false;
    this->speed = 0;
    this->publish_state();
  }

  fan::FanTraits get_traits() override {
    auto traits = fan::FanTraits();
    traits.set_speed(true);
    traits.set_supported_speed_count(100);
    traits.set_direction(true);
    return traits;
  }

  void control(const fan::FanCall &call) override {
    if (call.get_state().has_value()) {
      this->state = *call.get_state();
      if (this->state) {
        this->parent_->enable_motor(this->motor_id_);
      } else {
        this->parent_->disable_motor(this->motor_id_);
      }
    }
    if (call.get_speed().has_value()) {
      this->speed = *call.get_speed();
    }
    if (call.get_direction().has_value()) {
      this->direction = *call.get_direction();
    }

    if (this->state) {
      float target = (this->speed / 100.0f) * 20.0f;  // scale 0-100% to 0-20 rad/s
      if (this->direction == fan::FanDirection::REVERSE) {
        target = -target;
      }
      this->parent_->set_motor_speed(this->motor_id_, target);
    } else {
      this->parent_->set_motor_speed(this->motor_id_, 0);
    }

    this->publish_state();
  }

 protected:
  MakerbaseFOC *parent_{nullptr};
  int motor_id_{1};
};

}  // namespace makerbase_foc
}  // namespace esphome