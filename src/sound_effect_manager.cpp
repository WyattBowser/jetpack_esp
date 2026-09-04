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
    playSound(stateToSound(last_state));
  }
}

void SoundEffectManager::playSound(SFX sound) {
  //player.stop();
  switch (sound) {
    case FULL_CYCLE:
      Serial.println("SFX playing Full Cycle");
      player.play(1);
      break;
    case SPOOL_DOWN:
      Serial.println("SFX playing Spool Down");
      player.play(2);
      break;
    case DISARM:
      Serial.println("SFX playing No sound");
      player.stop();
      break;
    default:
      Serial.print("SFX tried playing unknown sfx: ");
      Serial.println(sound);
  }
}

SFX SoundEffectManager::stateToSound(JetpackState state) {
  switch(state) {
    case JetpackState::SPOOLING_UP: return SFX::FULL_CYCLE;
    case JetpackState::SPOOLING_DOWN: return SFX::SPOOL_DOWN;
    case JetpackState::DISARMED: return SFX::DISARM;
    default:
      Serial.print("SFX Manager cannot handle jetpack state: ");
      Serial.println(state);
      return SFX::DISARM;
  }
}