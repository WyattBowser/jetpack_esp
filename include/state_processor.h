#pragma once
#include <Arduino.h>
#include "jetpack_state.h"

class StateProcessor {
public:
  StateProcessor(JetpackState& state);
  void process();
  void triggerSpoolUp();
  void triggerSpoolDown();
  void triggerForcedDisarm();

private:
  JetpackState& shared_jetpack_state;
  JetpackState last_state;

  unsigned long state_start_time{0};
  static constexpr int NUM_TIMERS{3};      //Spool up, armed, spool down
  const unsigned long TIMERS[NUM_TIMERS] = {4500, 11500, 5000};
  bool user_triggered_spool_down = false;

  void initNewState();
};