#pragma once

#include "esphome.h"
#include <SimpleFOC.h>

class FOCController : public Component {
 public:

  BLDCMotor motor1 = BLDCMotor(7);
  BLDCMotor motor2 = BLDCMotor(7);

  BLDCDriver3PWM driver1 = BLDCDriver3PWM(
    32,
    33,
    25,
    22
  );

  BLDCDriver3PWM driver2 = BLDCDriver3PWM(
    26,
    27,
    14,
    12
  );

  float target1 = 0;
  float target2 = 0;

  void setup() override {

    driver1.voltage_power_supply = 12;
    driver2.voltage_power_supply = 12;

    driver1.init();
    driver2.init();

    motor1.linkDriver(&driver1);
    motor2.linkDriver(&driver2);

    motor1.controller = MotionControlType::velocity;
    motor2.controller = MotionControlType::velocity;

    motor1.voltage_limit = 3;
    motor2.voltage_limit = 3;

    motor1.velocity_limit = 20;
    motor2.velocity_limit = 20;

    motor1.PID_velocity.P = 0.2;
    motor1.PID_velocity.I = 2;
    motor1.PID_velocity.D = 0;

    motor2.PID_velocity.P = 0.2;
    motor2.PID_velocity.I = 2;
    motor2.PID_velocity.D = 0;

    motor1.init();
    motor2.init();

    motor1.enable();
    motor2.enable();

    ESP_LOGI("FOC", "FOC Controller initialized");
  }

  void loop() override {

    motor1.loopFOC();
    motor2.loopFOC();

    motor1.move(target1);
    motor2.move(target2);
  }

  void set_motor1_speed(float speed) {
    target1 = speed;
  }

  void set_motor2_speed(float speed) {
    target2 = speed;
  }

  void enable() {
    motor1.enable();
    motor2.enable();
  }

  void disable() {
    motor1.disable();
    motor2.disable();
  }
};
