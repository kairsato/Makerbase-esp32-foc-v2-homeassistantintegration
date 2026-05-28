#pragma once

#include "esphome.h"
#include <SimpleFOC.h>

// Makerbase ESP32 FOC v2 pin definitions
// Motor 1: UH=32, UL=33, VH=25, VL=26, WH=27, WL=14, EN=12
// Motor 2: UH=19, UL=18, VH=4, VL=2, WH=15, WL=13, EN=23

class FOCDriver : public Component {
 public:
  // Motor 1 - 6PWM driver (Makerbase uses 6PWM)
  BLDCMotor motor1_ = BLDCMotor(7);  // pole pairs - CHANGE to match your motor
  BLDCDriver6PWM driver1_ = BLDCDriver6PWM(32, 33, 25, 26, 27, 14);

  // Motor 2
  BLDCMotor motor2_ = BLDCMotor(7);  // pole pairs - CHANGE to match your motor
  BLDCDriver6PWM driver2_ = BLDCDriver6PWM(19, 18, 4, 2, 15, 13);

  float target1_ = 0;
  float target2_ = 0;
  bool enabled1_ = false;
  bool enabled2_ = false;

  void setup() override {
    // Motor 1
    driver1_.voltage_power_supply = 12;  // CHANGE to your supply voltage
    driver1_.init();
    motor1_.linkDriver(&driver1_);
    motor1_.voltage_limit = 5;
    motor1_.controller = MotionControlType::velocity_openloop;
    motor1_.init();

    // Motor 2
    driver2_.voltage_power_supply = 12;
    driver2_.init();
    motor2_.linkDriver(&driver2_);
    motor2_.voltage_limit = 5;
    motor2_.controller = MotionControlType::velocity_openloop;
    motor2_.init();

    ESP_LOGI("foc", "FOC Driver initialized");
  }

  void loop() override {
    if (enabled1_) {
      motor1_.move(target1_);
    } else {
      motor1_.move(0);
    }
    if (enabled2_) {
      motor2_.move(target2_);
    } else {
      motor2_.move(0);
    }
  }

  void set_speed(int motor, float speed) {
    if (motor == 1) target1_ = speed;
    else target2_ = speed;
    ESP_LOGI("foc", "Motor %d speed: %.2f", motor, speed);
  }

  void enable_motor(int motor) {
    if (motor == 1) enabled1_ = true;
    else enabled2_ = true;
    ESP_LOGI("foc", "Motor %d enabled", motor);
  }

  void disable_motor(int motor) {
    if (motor == 1) { enabled1_ = false; target1_ = 0; }
    else { enabled2_ = false; target2_ = 0; }
    ESP_LOGI("foc", "Motor %d disabled", motor);
  }
};