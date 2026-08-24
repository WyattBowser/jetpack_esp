#include "fog_machine_manager.h"

FogMachineManager::FogMachineManager(JetpackState& state) : shared_jetpack_state(state) {
  pinMode(PIN, OUTPUT);
}

void FogMachineManager::process() {
  if (last_state == shared_jetpack_state) { return; }

  last_state = shared_jetpack_state;

  switch (last_state) {
    case SPOOLING_UP:
      turnOn();
      break;

    case SPOOLING_DOWN:
      turnOff();
      break;
  }
}


void FogMachineManager::turnOn() { 
  Serial.println("Fog turning on");
  digitalWrite(PIN, HIGH); 
}

void FogMachineManager::turnOff() {
  Serial.println("Fog turning on");
  digitalWrite(PIN, LOW);
}