#include "bluetooth_manager.h"

BluetoothManager::BluetoothManager(StateProcessor& state_proc, JetpackState& state) :
  state_processor(state_proc), current_state(state), control_characteristic(CHARACTERISTIC_UUID, BLERead | BLEWrite) {
    last_connection_state = DISCONNECTED;
}

void BluetoothManager::init() {
  BLE.begin();
  delay(250);
  BLE.setLocalName(LOCAL_NAME);
  Serial.print("Local Name set to: ");
  Serial.println(LOCAL_NAME);

  BLEService new_service("69420");
  Serial.print("Service created: ");
  Serial.println(new_service.uuid());

  new_service.addCharacteristic(control_characteristic);
  Serial.print("Service now has characteristic: ");
  Serial.println(new_service.characteristic("6969").uuid());

  BLE.setAdvertisedService(new_service);
  BLE.addService(new_service);
  control_characteristic.writeValue((uint8_t)-1);
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

      if (control_characteristic.written()) {
        int control = (int)control_characteristic.value();
        switch (control) {
          case JetpackState::SPOOLING_UP:
            Serial.println("Jetpack BT got command to spool up");
            state_processor.triggerSpoolUp();
            break;
          case JetpackState::SPOOLING_DOWN:
            Serial.println("Jetpack BT got command to spool down");
            state_processor.triggerSpoolDown();
            break;
          case JetpackState::DISARMED:
            Serial.println("Jetpack BT got command to force stop");
            //hard stop everything
            state_processor.triggerForcedDisarm();
            break;
        }
      }
    } else {
      if (last_connection_state != DISCONNECTED) {
        Serial.println("Jetpack BT Disconnected from Bracer");
        last_connection_state = DISCONNECTED;
      }
      //Terminate everything for safety
    }
  }
}