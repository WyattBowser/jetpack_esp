#pragma once
#include "Adafruit_NeoPixel.h"

///
/// Fire simulator
///
class NeoFire
{
  Adafruit_NeoPixel &strip;
 public:

  NeoFire(Adafruit_NeoPixel&);
  void Draw();
  void Clear();
  void AddColor(uint8_t position, uint32_t color);
  void SubstractColor(uint8_t position, uint32_t color);
  uint32_t Blend(uint32_t color1, uint32_t color2);
  uint32_t Substract(uint32_t color1, uint32_t color2);

  uint32_t fire_color;
  uint32_t off_color;

};

