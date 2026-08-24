#include "jetpack_state.h"
#include "flame_led_ring_controller.h"
#include "Adafruit_NeoPixel.h"
class FlameEffectManager {
public:
  FlameEffectManager(JetpackState& state);
  void init();
  void process();

  void processLeds();
  void turnOff();

private:
  JetpackState& shared_jetpack_state;
  JetpackState last_state;

  const unsigned long LED_UPDATE_TIMER = 30;
  unsigned long last_led_update = 0;

  const int LEFT_THRUSTER_LED_COUNT = 8;
  const int LEFT_THRUSTER_PIN = D8;
  Adafruit_NeoPixel left_thruster_np;
  NeoFire left_thruster;

  const int CENTER_THRUSTER_LED_COUNT = 12;
  const int CENTER_THRUSTER_PIN = D9;
  Adafruit_NeoPixel center_thruster_np;
  NeoFire center_thruster;

  const int RIGHT_THRUSTER_LED_COUNT = 8;
  const int RIGHT_THRUSTER_PIN = D10;
  Adafruit_NeoPixel right_thruster_np;
  NeoFire right_thruster;
};