#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/fan/fan.h"

namespace esphome {
namespace makerbase_foc {

class MakerbaseFOC : public Component, public uart::UARTDevice {
 public:
  void setup() override {}
  void loop() override {}
};

class MakerbaseFOCFan : public Component, public fan::Fan {
 public:
  void set_parent(MakerbaseFOC *parent) { this->parent_ = parent; }

  fan::FanTraits get_traits() override {
    auto traits = fan::FanTraits();
    traits.set_speed(true);
    return traits;
  }

  void control(const fan::FanCall &call) override {
    if (call.get_state().has_value()) {
      this->state = *call.get_state();
    }
    if (call.get_speed().has_value()) {
      this->speed = *call.get_speed();
    }
    this->publish_state();
  }

 protected:
  MakerbaseFOC *parent_{nullptr};
};

}  // namespace makerbase_foc
}  // namespace esphome