#include "Measurement.h"

namespace measurement {
  entry operator+(const entry& e1, const entry& e2){
    entry result;
    result.time = e1.time;
    result.atm_temperature = e1.atm_temperature + e2.atm_temperature;
    result.atm_humidity = e1.atm_humidity + e2.atm_humidity;
    result.atm_air_pressure = e1.atm_air_pressure + e2.atm_air_pressure;
    result.atm_air_particle = e1.atm_air_particle + e2.atm_air_particle;
    result.atm_air_smoke = e1.atm_air_smoke + e2.atm_air_smoke;
    result.atm_CO2_ammount = e1.atm_CO2_ammount + e2.atm_CO2_ammount;
    result.atm_sound_pressure = e1.atm_sound_pressure + e2.atm_sound_pressure;
    result.light_intensity = e1.light_intensity + e2.light_intensity;
    result.UV_intensity = e1.UV_intensity + e2.UV_intensity;
    result.quality_rating = e1.quality_rating + e2.quality_rating;
    return result;
  }
  entry operator/(const entry& e, const int num){
    entry result;
    result.time = e.time;
    result.atm_temperature = e.atm_temperature / num;
    result.atm_humidity = e.atm_humidity / num;
    result.atm_air_pressure = e.atm_air_pressure / num;
    result.atm_air_particle = e.atm_air_particle / num;
    result.atm_air_smoke = e.atm_air_smoke / num;
    result.atm_CO2_ammount = e.atm_CO2_ammount / num;
    result.atm_sound_pressure = e.atm_sound_pressure / num;
    result.light_intensity = e.light_intensity / num;
    result.UV_intensity = e.UV_intensity / num;
    result.quality_rating = e.quality_rating / num;
    return result; 
  }
  void print_entry_binary(entry e) {
    Serial.write((byte*)&e, sizeof(e));
  }
  void print_entry(entry e) {
    Serial.print(e.time);
    Serial.print(",");
    Serial.print(e.atm_temperature);
    Serial.print(",");
    Serial.print(e.atm_humidity);
    Serial.print(",");
    Serial.print(e.atm_air_pressure);
    Serial.print(",");
    Serial.print(e.atm_air_particle);
    Serial.print(",");
    Serial.print(e.atm_air_smoke);
    Serial.print(",");
    Serial.print(e.atm_CO2_ammount);
    Serial.print(",");
    Serial.print(e.atm_sound_pressure);
    Serial.print(",");
    Serial.print(e.light_intensity);
    Serial.print(",");
    Serial.print(e.UV_intensity);
    Serial.print(",");
    Serial.print(e.quality_rating);
    Serial.print(",");
    Serial.println(e.user_likes);
  }

  void print_headers() {
    Serial.print("time");
    Serial.print(",");
    Serial.print("atm_temperature");
    Serial.print(",");
    Serial.print("atm_humidity");
    Serial.print(",");
    Serial.print("atm_air_pressure");
    Serial.print(",");
    Serial.print("atm_air_particle");
    Serial.print(",");
    Serial.print("atm_air_smoke");
    Serial.print(",");
    Serial.print("atm_CO2_ammount");
    Serial.print(",");
    Serial.print("atm_sound_pressure");
    Serial.print(",");
    Serial.print("light_intensity");
    Serial.print(",");
    Serial.print("UV_intensity");
    Serial.print(",");
    Serial.println("quality_rating,user_likes");
  }

};
