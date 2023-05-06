#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <Arduino.h>

namespace measurement {
  const float NOVAL = 0;
  enum LIKES {
    NO,
    YES,
    UNKNOWN
  };
  struct entry {
    unsigned long time = 0;
    float atm_temperature = NOVAL;
    float atm_humidity = NOVAL;
    float atm_air_pressure = NOVAL;
    float atm_air_particle = NOVAL;
    float atm_air_smoke = NOVAL;
    float atm_CO2_ammount = NOVAL;
    float atm_sound_pressure = NOVAL;
    float light_intensity = NOVAL;
    float UV_intensity = NOVAL;
    float quality_rating = 100;
    short int user_likes = UNKNOWN;
  };
  
  entry operator+(const entry& e1, const entry& e2);
  entry operator/(const entry& e, const int num);
  void print_entry_binary(entry e);
  void print_entry(entry e);
  void print_headers();
};

#endif // MEASUREMENT_H