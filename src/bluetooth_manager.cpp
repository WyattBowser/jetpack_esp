#include "bluetooth_manager.h"

BluetoothManager* BluetoothManager::instance = nullptr;

BluetoothManager::BluetoothManager(StateProcessor& state_proc, JetpackState& state) :
  state_processor(state_proc),
  current_state(state),
  control_characteristic(CONTROL_UUID, BLEWrite),
  state_characteristic(STATE_UUID, BLERead | BLENotify) {
    last_connection_state = DISCONNECTED;
    instance = this;
}

void BluetoothManager::init() {
  BLE.begin();
  delay(250);
  BLE.setLocalName(LOCAL_NAME);
  Serial.print("Local Name set to: ");
  Serial.println(LOCAL_NAME);

  BLEService new_service(SERVICE_UUID);
  Serial.print("Service created: ");
  Serial.println(new_service.uuid());

  new_service.addCharacteristic(control_characteristic);
  Serial.print("Service now has characteristic: ");
  Serial.println(new_service.characteristic(CONTROL_UUID).uuid());
  new_service.addCharacteristic(state_characteristic);
  Serial.print("Service now has characteristic: ");
  Serial.println(new_service.characteristic(STATE_UUID).uuid());

  BLE.setAdvertisedService(new_service);
  BLE.addService(new_service);
  control_characteristic.writeValue((uint8_t)-1);
  control_characteristic.setEventHandler(BLEWritten, handleCommandStatic);
  BLE.advertise();
  
  Serial.println(new_service.characteristicCount());
}

void BluetoothManager::process() {
  if (!central) {
    central = BLE.central();
  } else {
    if (central.connected()) {
      if (last_connection_state != CONNECTED) {
        Serial.println("Jetpack BT is now connected to bracer");
        last_connection_state = CONNECTED;
      }
    } else {
      if (last_connection_state != DISCONNECTED) {
        last_connection_state = DISCONNECTED;
        Serial.println("Jetpack BT has central, but has disconnected");
        state_processor.triggerForcedDisarm();
      }
    }
  }
  
  if (!central.connected() && last_connection_state != DISCONNECTED) {
      last_connection_state = DISCONNECTED;
      Serial.println("Jetpack BT has central, but never connected");
      state_processor.triggerForcedDisarm();
  }

  if (central && central.connected()) {
    unsigned long current_time = millis();
    if (current_time - last_report_time_ > STATE_REPORT_TIMEOUT_MS) {
      state_characteristic.writeValue(current_state);
      last_report_time_ = current_time;
    }
  }
}

void BluetoothManager::handleCommandStatic(BLEDevice central, BLECharacteristic chr) {
  instance->handleCommand(central, chr);
}

void BluetoothManager::handleCommand(BLEDevice central, BLECharacteristic chr) {
  JetpackCommand command = (JetpackCommand)control_characteristic.value();
      Serial.print("Jetpack BT got command: ");
      Serial.println(toString(command));
      switch (command) {
        case JetpackCommand::FULL_CYCLE:
          state_processor.triggerSpoolUp();
          break;
        case JetpackCommand::SPOOL_DOWN:
          state_processor.triggerSpoolDown();
          break;
        case JetpackCommand::STOP:
          //hard stop everything
          state_processor.triggerForcedDisarm();
          break;
        default:
          Serial.println("Jetpack BT cannot handle command. Skipping");
      }
}