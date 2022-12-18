namespace measurement {
  const float NOVAL = -100;
    enum LIKES {
    NO,
    YES,
    UNKNOWN
  };
  struct entry {
    float time = 0;
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
  entry operator+(const entry& e1, const entry& e2){
    entry result;
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
};
