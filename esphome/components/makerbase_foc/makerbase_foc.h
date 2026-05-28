#pragma once

#include "esphome/core/component.h"
#include <SimpleFOC.h>

namespace esphome {
namespace makerbase_foc {

class MakerbaseFOC : public Component {
 public:
  BLDCMotor motor1_{7};  // CHANGE pole pairs
  BLDCDriver6PWM driver1_{32, 33, 25, 26, 27, 14};

  BLDCMotor motor2_{7};  // CHANGE pole pairs
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

}  // namespace makerbase_foc
}  // namespace esphome