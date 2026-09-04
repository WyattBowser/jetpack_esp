#include "flame_effect_manager.h"

FlameEffectManager::FlameEffectManager(JetpackState& state) :
  shared_jetpack_state(state),
  left_thruster_np(LEFT_THRUSTER_LED_COUNT, LEFT_THRUSTER_PIN, NEO_GRB + NEO_KHZ800),
  left_thruster(left_thruster_np),
  center_thruster_np(CENTER_THRUSTER_LED_COUNT, CENTER_THRUSTER_PIN, NEO_GRB + NEO_KHZ800),
  center_thruster(center_thruster_np),
  right_thruster_np(RIGHT_THRUSTER_LED_COUNT, RIGHT_THRUSTER_PIN, NEO_GRB + NEO_KHZ800),
  right_thruster(right_thruster_np){
    last_state = shared_jetpack_state;
  }

void FlameEffectManager::init() {
  left_thruster_np.begin();
  left_thruster_np.show();
  center_thruster_np.begin();
  center_thruster_np.show();
  right_thruster_np.begin();
  right_thruster_np.show();
}


void FlameEffectManager::process() {
  processLeds();

  if (last_state == shared_jetpack_state) { return; }
  last_state = shared_jetpack_state;
  Serial.println("Flame effect manager sensed state change");

  switch(last_state) {
    case ARMED:
      Serial.println("Flame Effect: ARMED");
      break;
    case SPOOLING_DOWN:
      turnOff();
      break;
    case DISARMED:
      turnOff();
      break;
  }
}

void FlameEffectManager::processLeds() {
  if (last_state != ARMED) { return; }
  if (millis() - last_led_update < LED_UPDATE_TIMER) { return; }
  last_led_update = millis();
  right_thruster.Draw();
  center_thruster.Draw();
  left_thruster.Draw();
}

void FlameEffectManager::turnOff() {
  right_thruster.Clear();
  right_thruster_np.show();
  center_thruster.Clear();
  center_thruster_np.show();
  left_thruster.Clear();
  left_thruster_np.show();

}