#pragma once

#include "esphome/core/component.h"
#include "esphome/components/fan/fan.h"

namespace esphome {
namespace makerbase_foc {

class MakerbaseFOCFan : public Component, public fan::Fan {
 public:
  void setup() override {}
  void loop() override {}
};

}  // namespace makerbase_foc
}  // namespace esphome
