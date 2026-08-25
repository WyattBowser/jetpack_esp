#include <ArduinoBLE.h>
#include <jetpack_state.h>
#include <state_processor.h>

class StateProcessor;

enum ConnectionState {
  CONNECTED,
  DISCONNECTED
};

class BluetoothManager {
public:
  BluetoothManager(StateProcessor& state_proc, JetpackState& state);
  void init();
  void process();

private:
  const char* LOCAL_NAME = "Hijack Jetpack";
  const char* CHARACTERISTIC_UUID = "6969";
  BLEIntCharacteristic control_characteristic;
  BLEDevice central;

  StateProcessor& state_processor;
  const JetpackState& current_state;
  
  ConnectionState last_connection_state;
};