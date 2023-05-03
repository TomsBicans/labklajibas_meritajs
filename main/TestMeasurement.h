#include <ArduinoUnit.h>
#include "Measurement.h"

bool float_compare(float x, float y, float tolerance) {
    if (abs(x - y) < tolerance) {
      return true;
    } else {
      return false;
    }
  }


test(entry_struct) {
  // create an entry struct
  measurement::entry e = measurement::entry{};
  Serial.print("test_entry_struct ");

  // check that the fields are set to the expected default values
  assert(e.atm_temperature == measurement::NOVAL);
  assert(e.atm_air_pressure == measurement::NOVAL);
  assert(e.atm_air_particle == measurement::NOVAL);
  assert(e.atm_CO2_ammount == measurement::NOVAL);
  Serial.print(".");
  assert(e.atm_sound_pressure == measurement::NOVAL);
  assert(e.UV_intensity == measurement::NOVAL);
  assert(e.quality_rating == 100);
  // assert(e.user_likes == measurement::LIKES::UNKNOWN);
  Serial.print(".");
}

test(entry_struct_values) {
  // create an entry struct
  measurement::entry e = measurement::entry{};

  // set the values of some of the fields
  e.atm_temperature = 15.5;
  e.atm_air_pressure = 1013.2;
  e.atm_air_particle = 0.8;
  e.atm_CO2_ammount = 400;
  e.atm_sound_pressure = 70;

  // check that the fields have the expected values
  assert(float_compare(e.atm_temperature, 15.5, 1));
  assert(float_compare(e.atm_air_pressure, 1013.2, 1));
  assert(float_compare(e.atm_air_particle, 0.8, 1));
  assert(float_compare(e.atm_CO2_ammount, 400, 1));
  assert(float_compare(e.atm_sound_pressure, 70, 1));
  // Serial.print(".");

  // check that the other fields still have the default values
  assert(float_compare(e.UV_intensity, measurement::NOVAL, 1));
  assert(float_compare(e.quality_rating, 100, 1));
  // assert(e.user_likes == measurement::UNKNOWN);
}

test(entry_struct_addition) {
  // create an entry struct
  measurement::entry e1 = measurement::entry{};
  measurement::entry e2 = measurement::entry{};
  measurement::entry e3 = measurement::entry{};
  // set the values of some of the fields
  e1.atm_temperature = 50;
  e1.atm_air_pressure = 50;
  e1.atm_air_particle = 50;
  e1.atm_CO2_ammount = 50;
  e1.atm_sound_pressure = 50;

  e2.atm_temperature = 50;
  e2.atm_air_pressure = 50;
  e2.atm_air_particle = 50;
  e2.atm_CO2_ammount = 50;
  e2.atm_sound_pressure = 50;

  e3 = e1 + e2;
  // print_entry(e3);

  // check that the fields have the expected values

  assert(float_compare(e3.atm_temperature, 100, 1));
  assert(float_compare(e3.atm_air_pressure, 100, 1));
  assert(float_compare(e3.atm_air_particle, 100, 1));
  assert(float_compare(e3.atm_CO2_ammount, 100, 1));
  assert(float_compare(e3.atm_sound_pressure, 100, 1));
}