#pragma once
enum JetpackState {
  SPOOLING_UP,      //SFX, SMOKE
  ARMED,            //SFX, SMOKE, LIGHTS
  SPOOLING_DOWN,    //SFX, SMOKE
  DISARMED          //ALL OFF
};

/* CAUTION: THERE IS NO CLAMPING! we can go past disarmed and get an error */
inline JetpackState& operator++(JetpackState& state) {
  state = static_cast<JetpackState>(static_cast<int>(state) + 1);
  return state;
}


/* CAUTION: THERE IS NO CLAMPING! we can go past disarmed and get an error */
inline JetpackState operator++(JetpackState& state, int) {
  JetpackState old = state;
  ++state;
  return old;
}

inline const char* toString(JetpackState state) {
  switch (state) {
    case SPOOLING_UP:   return "SPOOLING_UP";
    case ARMED:         return "ARMED";
    case SPOOLING_DOWN: return "SPOOLING_DOWN";
    case DISARMED:      return "DISARMED";
  }
  return "UNKNOWN";
}