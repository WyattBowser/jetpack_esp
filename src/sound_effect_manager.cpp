#include "sound_effect_manager.h"

SoundEffectManager::SoundEffectManager(JetpackState& state) : 
    shared_jetpack_state(state), last_state(state), DFSerial(1) {
  DFSerial.begin(9600, SERIAL_8N1, D5, D6);
  player.begin(DFSerial);
  player.volume(30);
}

void SoundEffectManager::process() {
  // Drain unsolicited DFPlayer messages (e.g. play-finished) so they don't
  // pile up in the UART buffer and desync the ACK handshake used by play().
  while (player.available()) {
    player.readType();
    player.read();
  }

  if (last_state == shared_jetpack_state) {
    return;
  } else {
    Serial.println("SFX sensed State Changed");
    last_state = shared_jetpack_state;
  }

  //We dont handle ARMED state as there its sound effect is baked into the full cycle sound effect
  if (last_state != ARMED) {
    playSoundFromState(last_state);
  }
}

void SoundEffectManager::playSoundFromState(JetpackState state) {
  Serial.print("SFX playing sound for: ");
  Serial.println(toString(state));
  switch (state) {
    case JetpackState::SPOOLING_UP:
      player.play(1);
      break;
    case JetpackState::ARMED:
      Serial.println("SFX does not do anything for armed state");
      break;
    case JetpackState::USER_SPOOL_DOWN:
      player.play(2);
      break;
    case JetpackState::SPOOLING_DOWN:
      Serial.println("SFX does not do anything for Spool down state");
      break; //Do nothing herer. The master sound clip has the spool down sfx alread in it
    case JetpackState::DISARMED:
      player.stop();
      break;
    default:
      Serial.print("SFX tried playing unknown sfx. Skipping");
  }
}