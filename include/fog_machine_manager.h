#pragma once
#include <Arduino.h>
#include "jetpack_state.h"
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