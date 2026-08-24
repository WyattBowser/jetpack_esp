#pragma once
#include <Arduino.h>
#include "jetpack_state.h"
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
  const int CHOKE_POS = 45;
  const int OPEN_POS = 90;
};