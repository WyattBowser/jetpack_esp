#include <Arduino.h>

#include "jetpack_deps/common.h"
#include "state_processor.h"
#include "sound_effect_manager.h"
#include "fog_machine_manager.h"
#include "flame_effect_manager.h"
#include "variable_nozzle_manager.h"
#include "bluetooth_manager.h"

JetpackState current_state = DISARMED;
StateProcessor state_processor(current_state);
SoundEffectManager sfx_manager(current_state);
FogMachineManager fog_manager(current_state);
FlameEffectManager flame_manager(current_state);
VariableNozzleManager nozzle_manager(current_state);
BluetoothManager bt_manager(state_processor, current_state);

void processStateProcessor(void* params) {
  StateProcessor* state_processor = static_cast<StateProcessor*>(params);
  Serial.println("Starting state Processor...");

  for(;;) {
     /*
    //Manual Trigger for testing purposes
    if (current_state == DISARMED) {
      Serial.println("Resetting State...");
      vTaskDelay(pdMS_TO_TICKS(3000));
      current_state = SPOOLING_UP;
      Serial.println("Reset Complete");
    }
    */
    state_processor->process();
    vTaskDelay(1);
  }
}

void processSoundEffectsManager(void* params) {
  SoundEffectManager* sfx_manager = static_cast<SoundEffectManager*>(params);
  Serial.println("Starting SFX processing...");
  for(;;) {
    sfx_manager->process();
    vTaskDelay(1);
  }
}

void processFogMachineManager(void* params) {
  FogMachineManager* fog_manager = static_cast<FogMachineManager*>(params);
  Serial.println("Starting fog machine processing...");
  for(;;) {
    fog_manager->process();
    vTaskDelay(1);
  }
}

void processFlameEffectManager(void* params) {
  FlameEffectManager* flame_manager = static_cast<FlameEffectManager*>(params);
  Serial.println("Starting flame effect processing...");
  for (;;) {
    flame_manager->process();
    vTaskDelay(1);
  }
}

void processVariableNozzleManager(void* params) {
  VariableNozzleManager* nozzle_manager = static_cast<VariableNozzleManager*>(params);
  Serial.println("Starting Nozzle processing...");
  for (;;) {
    nozzle_manager->process();
    vTaskDelay(1);
  }
}

void processBluetoothManager(void* params) {
  BluetoothManager* bt_manager = static_cast<BluetoothManager*>(params);
  Serial.println("Starting BT Manager processing"); 
  for (;;) {
    bt_manager->process();
    vTaskDelay(1);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  flame_manager.init();
  bt_manager.init();
  xTaskCreate(processStateProcessor, "State Processing", 4096, &state_processor, 0, nullptr);
  xTaskCreate(processSoundEffectsManager, "SFX Processing", 8192, &sfx_manager, 0, nullptr);
  xTaskCreate(processFogMachineManager, "Fog Machine Processing", 4096, &fog_manager, 0, nullptr);
  xTaskCreate(processFlameEffectManager, "Flame Effect Processing", 4096, &flame_manager, 0, nullptr);
  xTaskCreate(processVariableNozzleManager, "Variable Nozzle Processing", 4096, &nozzle_manager, 0, nullptr);
  xTaskCreate(processBluetoothManager, "BT Processing", 4096, &bt_manager, 0, nullptr);
}

void loop() {
  Serial.println("HB");
  vTaskDelay(1000);
}