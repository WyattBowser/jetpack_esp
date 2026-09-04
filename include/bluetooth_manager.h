#include <ArduinoBLE.h>
#include "jetpack_deps/common.h"
#include "jetpack_deps/bluetooth.h"
#include "state_processor.h"

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
  void handleCommand(BLEDevice central, BLECharacteristic chr);

private:
  BLEDevice central;
  const char* SERVICE_UUID = "69420";

  BLEIntCharacteristic control_characteristic;

  BLEIntCharacteristic state_characteristic;
  const unsigned long STATE_REPORT_TIMEOUT_MS = 250;
  unsigned long last_report_time_ = 0;


  StateProcessor& state_processor;
  const JetpackState& current_state;
  
  ConnectionState last_connection_state;
};