#pragma once
#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include "jetpack_state.h"

enum SFX {
  FULL_CYCLE, //In charge of ENTIRE cycle
  SPOOL_DOWN, //Usually only used when user calls early spooldown
  DISARM  //Resting state
};

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
  bool user_triggered_spool_down{false};

  void playSound(SFX sfx);
};