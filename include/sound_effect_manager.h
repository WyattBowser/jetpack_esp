#pragma once
#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include "jetpack_deps/common.h"

class SoundEffectManager {
public:
  SoundEffectManager(JetpackState& state);
  void process();
  void userTriggeredSpooldown();

private:
  HardwareSerial DFSerial;
  DFRobotDFPlayerMini player;

  const JetpackState& shared_jetpack_state;
  JetpackState last_state;

  void playSoundFromState(JetpackState sfx);
};