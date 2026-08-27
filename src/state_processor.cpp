#include "state_processor.h"

StateProcessor::StateProcessor(JetpackState& state) : shared_jetpack_state(state), last_state(state){};

void StateProcessor::process() {
  /*
  if (shared_jetpack_state == DISARMED) { return; }
  if (user_triggered_spool_down) {
    Serial.println("User Triggered Spooldown"); 
    if (shared_jetpack_state == SPOOLING_DOWN) {
      shared_jetpack_state = DISARMED;
    } else {
      shared_jetpack_state = SPOOLING_DOWN;
    }
    initNewState();
    user_triggered_spool_down = false;
    return;
  }
  */

  if(last_state != shared_jetpack_state) {
    initNewState();
  }

  if (millis() - state_start_time > TIMERS[shared_jetpack_state]) {
      if (shared_jetpack_state != DISARMED) {
      shared_jetpack_state++;
      initNewState();
    }
  }
}

void StateProcessor::initNewState() {
  last_state = shared_jetpack_state;
  state_start_time = millis();
  Serial.print("New State: ");
  Serial.println(toString(shared_jetpack_state));
}

void StateProcessor::triggerSpoolDown() {
  shared_jetpack_state = SPOOLING_DOWN;
}

void StateProcessor::triggerSpoolUp() {
  if (shared_jetpack_state == SPOOLING_DOWN) {
    Serial.println("State processor is already spooling down. Forcing shutdown");
    triggerForcedDisarm();
  } else {
    shared_jetpack_state = SPOOLING_UP;
  }
}

void StateProcessor::triggerForcedDisarm() {
  shared_jetpack_state = DISARMED;
}