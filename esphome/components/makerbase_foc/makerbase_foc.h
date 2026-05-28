#pragma once

#include "esphome/core/component.h"
#include "esphome/components/fan/fan.h"
#include <SimpleFOC.h>

namespace esphome {
namespace makerbase_foc {

class MakerbaseFOC : public Component {
 public:
  BLDCMotor motor1_{7};
  BLDCDriver6PWM driver1_{32, 33, 25, 26, 27, 14};

  BLDCMotor motor2_{7};
  BLDCDriver6PWM driver2_{19, 18, 4, 2, 15, 13};

  float target1_ = 0, target2_ = 0;
  bool enabled1_ = false, enabled2_ = false;

  void setup() override {
    driver1_.voltage_power_supply = 12;
    driver1_.init();
    motor1_.linkDriver(&driver1_);
    motor1_.voltage_limit = 5;
    motor1_.controller = MotionControlType::velocity_openloop;
    motor1_.init();

    driver2_.voltage_power_supply = 12;
    driver2_.init();
    motor2_.linkDriver(&driver2_);
    motor2_.voltage_limit = 5;
    motor2_.controller = MotionControlType::velocity_openloop;
    motor2_.init();
  }

  void loop() override {
    motor1_.move(enabled1_ ? target1_ : 0);
    motor2_.move(enabled2_ ? target2_ : 0);
  }

  void set_speed(int motor, float speed) {
    if (motor == 1) target1_ = speed; else target2_ = speed;
  }
  void enable_motor(int motor) {
    if (motor == 1) enabled1_ = true; else enabled2_ = true;
  }
  void disable_motor(int motor) {
    if (motor == 1) { enabled1_ = false; target1_ = 0; }
    else { enabled2_ = false; target2_ = 0; }
  }
};

class MakerbaseFOCFan : public Component, public fan::Fan {
 public:
  void set_parent(MakerbaseFOC *parent) { parent_ = parent; }
  void set_motor_id(int id) { motor_id_ = id; }

  void setup() override {
    this->state = false;
    this->speed = 0;
    this->publish_state();
  }

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

}  // namespace makerbase_foc
}  // namespace esphome