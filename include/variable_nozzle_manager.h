#pragma once
#include <Arduino.h>
#include "jetpack_deps/common.h"
#include "async_servo.h"
class VariableNozzleManager {
public:
  VariableNozzleManager(JetpackState& state);
  void process();


private:
  JetpackState& shared_jetpack_state;
  JetpackState last_state;

  AsyncServo servo;
  int PIN = D0;
  const int CHOKE_POS = 90;
  const int OPEN_POS = 35;
};