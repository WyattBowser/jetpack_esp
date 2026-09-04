#pragma once
#include <Arduino.h>
#include "jetpack_deps/common.h"
class FogMachineManager {
public:
  FogMachineManager(JetpackState& state);
  void process();

private:
  JetpackState& shared_jetpack_state;
  JetpackState last_state;
  const uint8_t PIN = D7;

  void turnOn();
  void turnOff();
};