#include "Sensors.h"
#include "Timer.h"

namespace sensors{
  float atm_air_quality(int pin, unsigned long sampletime_ms){
    //https://www.howmuchsnow.com/arduino/airquality/grovedust/
    unsigned long duration;
    unsigned long starttime;
    unsigned long lowpulseoccupancy = 0;
    float ratio = 0;
    float concentration = 0;
    Timer t = Timer(sampletime_ms);
    while(!t.hasElapsed()){
      duration = pulseIn(pin, LOW);
      lowpulseoccupancy += duration;
    }
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62;
    if (concentration <= 0.62){
      return 0;
    }
    concentration *= 100; // Convert pcs/0.01cf => pcs/cf
    concentration /= 0.0283168466; // Convert pcs/cf => pcs/m^3
    concentration /= 1000; // Convert pcs/m^3 => pcs/L
    return concentration;
    }
  float UV_intensity(int pin){
    /*TODO: implement */
    return 0;
    }
  float light_intensity(int pin){
    /*TODO: implement */
    return 0;
    }
  float atm_CO2_ammount(int pin){
    /*TODO: implement */
    return 0;
    float res = 0;
    float count = 0;
    for (int i=0; i<4; i++){
      unsigned long sample_time = 1004; // ms
      float time_high = 2;
      float time_low = 2;
      unsigned long duration;
      float result;    
      Timer t = Timer(sample_time);
      while (!t.hasElapsed()){
        duration = pulseIn(pin, LOW);
        time_low += duration;
      }
      time_high = sample_time - time_low - 2;
      // time_low = sample_time - time_high - 2;
      result = 5000*(time_high-2)/(time_high+time_low-4);
      res += result;
      count += 1;
    }
    return res/count;
    }
  float atm_air_pressure(Adafruit_BMP085 &sensor){
    /*TODO: implement */
    float res = sensor.readPressure();
    res = res / 1000; // Pa -> kPa
    return res;
    }
  float atm_smoke_ammount(int pin){
    /*TODO: implement */
    return 0;
    }
  float atm_sound_pressure(int pin){
    float res = 0;
    int count = 0;
    Timer t = Timer(3*1000);
    while(!t.hasElapsed()){
      res += analogRead(pin);
      count++;
      delay(50);
    }
    res /= count;
    return res;
    }
};
