#pragma once

#include "esphome/core/component.h"
#include "esphome/components/fan/fan.h"
#include <SimpleFOC.h>
#include <Wire.h>

namespace esphome {
namespace makerbase_foc {

class MakerbaseFOC : public Component {
 public:
  // Motor instances - 3PWM drivers (not 6PWM!)
  BLDCMotor motor1_{7};
  BLDCDriver3PWM driver1_{32, 33, 25, 12};

  BLDCMotor motor2_{7};
  BLDCDriver3PWM driver2_{26, 27, 14, 12};

  // AS5600 encoders
  MagneticSensorI2C sensor1_{AS5600_I2C};
  TwoWire i2c1_ = TwoWire(0);
  MagneticSensorI2C sensor2_{AS5600_I2C};
  TwoWire i2c2_ = TwoWire(1);

  // Current sensing
  InlineCurrentSense current_sense1_{0.01f, 50.0f, 39, 36};
  InlineCurrentSense current_sense2_{0.01f, 50.0f, 35, 34};

  float target1_ =  5.0f, target2_ =  5.0f;
  bool enabled1_ = true, enabled2_ = true;

  float get_vin_volt() {
    return analogReadMilliVolts(13) * 8.5f / 1000.0f;
  }

  void setup() override {
    // Board init
    pinMode(32, INPUT_PULLUP);
    pinMode(33, INPUT_PULLUP);
    pinMode(25, INPUT_PULLUP);
    pinMode(26, INPUT_PULLUP);
    pinMode(27, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
    analogReadResolution(12);

    float vin = get_vin_volt();
    ESP_LOGI("foc", "Supply voltage: %.2fV", vin);

    // Encoder setup
    i2c1_.begin(19, 18, 400000UL);
    i2c2_.begin(23, 5, 400000UL);
    sensor1_.init(&i2c1_);
    sensor2_.init(&i2c2_);

    motor1_.linkSensor(&sensor1_);
    motor2_.linkSensor(&sensor2_);

    // Driver setup
    driver1_.voltage_power_supply = vin;
    driver1_.init();
    motor1_.linkDriver(&driver1_);

    driver2_.voltage_power_supply = vin;
    driver2_.init();
    motor2_.linkDriver(&driver2_);

    // Limits
    motor1_.current_limit = 0.5f;
    motor1_.voltage_limit = 0.5f;
    motor2_.current_limit = 0.5f;
    motor2_.voltage_limit = 0.5f;

    // Current sensing
    current_sense1_.init();
    motor1_.linkCurrentSense(&current_sense1_);
    current_sense2_.init();
    motor2_.linkCurrentSense(&current_sense2_);

    // Control mode
    motor1_.torque_controller = TorqueControlType::foc_current;
    motor1_.controller = MotionControlType::velocity;
    motor2_.torque_controller = TorqueControlType::foc_current;
    motor2_.controller = MotionControlType::velocity;

    motor1_.voltage_sensor_align = 5;
    motor2_.voltage_sensor_align = 5;

    // PID current
    motor1_.PID_current_q.P = 1;
    motor1_.PID_current_q.I = 500;
    motor1_.PID_current_d.P = 1;
    motor1_.PID_current_d.I = 500;
    motor1_.LPF_current_q.Tf = 0.002f;
    motor1_.LPF_current_d.Tf = 0.002f;

    motor2_.PID_current_q.P = 1;
    motor2_.PID_current_q.I = 500;
    motor2_.PID_current_d.P = 1;
    motor2_.PID_current_d.I = 500;
    motor2_.LPF_current_q.Tf = 0.002f;
    motor2_.LPF_current_d.Tf = 0.002f;

    // PID velocity
    motor1_.PID_velocity.P = 0.021f;
    motor1_.PID_velocity.I = 0.12f;
    motor1_.PID_velocity.D = 0;
    motor2_.PID_velocity.P = 0.021f;
    motor2_.PID_velocity.I = 0.12f;
    motor2_.PID_velocity.D = 0;

    // Speed limit
    motor1_.velocity_limit = 20;
    motor2_.velocity_limit = 20;

    // Init motors
    motor1_.init();
    motor1_.initFOC();
    motor2_.init();
    motor2_.initFOC();

    motor1_.target = 0;
    motor2_.target = 0;

    ESP_LOGI("foc", "Motors initialized");
  }

  void loop() override {
    motor1_.loopFOC();
    motor2_.loopFOC();

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
      float spd = (this->speed / 100.0f) * 20.0f;  // 0-100% maps to 0-20 rad/s
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