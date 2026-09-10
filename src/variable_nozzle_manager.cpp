#include "variable_nozzle_manager.h"

VariableNozzleManager::VariableNozzleManager(JetpackState& state) : 
  shared_jetpack_state(state) {
    last_state = shared_jetpack_state;
    servo.attach(PIN, CHOKE_POS);
}

void VariableNozzleManager::process() {
  //todo async servo::update/animation update
  servo.update();

  if (last_state == shared_jetpack_state) { return; }
  if (last_state == DISARMED) {
    Serial.println("Variable Nozzle sensed Disarmed. Not moving Servos");
  }

  last_state = shared_jetpack_state;
  Serial.println("Variable Nozzle Sensed State Change");

  switch(last_state) {
    case SPOOLING_UP:
      //Choke
      servo.moveTo(CHOKE_POS, 200);
      break;
    case ARMED:
      //open up
      servo.moveTo(OPEN_POS, 200);
      break;
    case SPOOLING_DOWN:
      //Choke
      servo.moveTo(CHOKE_POS, 1);
      break;
  }
}