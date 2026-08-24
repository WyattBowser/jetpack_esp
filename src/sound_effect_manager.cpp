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

  if(user_triggered_spool_down) {
    Serial.println("SFX User Triggered SPooldown");
    if (last_state == SPOOLING_DOWN) {
      playSound(DISARM);
    } else {
      playSound(SPOOL_DOWN);
    }
    user_triggered_spool_down = false;
    return;
  }

  switch(last_state) {
    case SPOOLING_UP:
      playSound(FULL_CYCLE);
      break;
    case DISARMED:
      //Make sure all sound is done
      playSound(DISARM);
      break;
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
      break;
  }
}

void SoundEffectManager::userTriggeredSpooldown() {
  user_triggered_spool_down = true;
}