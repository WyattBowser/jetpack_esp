#pragma once
#include <Arduino.h>
#include <ESP32Servo.h>

class AsyncServo {
  static constexpr int MIN_US = 500;
  static constexpr int MAX_US = 2400;

  Servo servo;
  int   lastDeg = 90;            // last fully-reached/target angle
  int   startDeg = 90;
  int   targetDeg = 90;
  float lastUs = 0;              // last microsecond pulse width actually written
  unsigned long startMs = 0;
  unsigned long durationMs = 0;
  bool moving = false;

  static float degToUs(int deg) {
    return MIN_US + (MAX_US - MIN_US) * (deg / 180.0f);
  }

public:

  void attach(int pin) {
    servo.setPeriodHertz(50);
    servo.attach(pin, MIN_US, MAX_US);
    lastUs = degToUs(lastDeg);
    startMs = millis();
  }

  void attach(int pin, int startAngle) {
    servo.setPeriodHertz(50);
    servo.attach(pin, MIN_US, MAX_US);
    lastDeg = startDeg = targetDeg = constrain(startAngle, 0, 180);
    lastUs = degToUs(lastDeg);
    startMs = millis();
    moveTo(startAngle, 2000);
  }

  // schedule a move to 'deg' that takes 'ms' milliseconds
  void moveTo(int deg, unsigned long ms) {
    deg = constrain(deg, 0, 180);
    if (ms == 0) {               // instant
      targetDeg = startDeg = lastDeg = deg;
      lastUs = degToUs(lastDeg);
      servo.writeMicroseconds((int)(lastUs + 0.5f));
      moving = false;
      return;
    }
    startDeg   = lastDeg;        // begin from current commanded angle
    targetDeg  = deg;
    startMs    = millis();
    durationMs = ms;
    moving     = true;
  }

  // call often (each loop). returns true if currently moving
  bool update() {
    if (!moving) return false;
    unsigned long now = millis();
    unsigned long elapsed = now - startMs;
    if (elapsed >= durationMs) {
      lastDeg = targetDeg;
      lastUs = degToUs(lastDeg);
      servo.writeMicroseconds((int)(lastUs + 0.5f));
      moving = false;
      return false;
    }
    // ease-in/ease-out interpolation, done in microseconds for sub-degree resolution
    float t = (float)elapsed / (float)durationMs;          // 0..1, linear progress
        t = t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);       // smootherstep: gentler than smoothstep, no jerk at the ends
    float usStart  = degToUs(startDeg);
    float usTarget = degToUs(targetDeg);
    float cmdUs    = usStart + (usTarget - usStart) * t;
    float diff     = cmdUs - lastUs;
    if (diff > 1.0f || diff < -1.0f) {
      lastUs = cmdUs;
      servo.writeMicroseconds((int)(cmdUs + 0.5f));
    }
    return true;
  }
};


