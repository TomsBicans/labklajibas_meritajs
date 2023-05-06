#ifndef SENSORS_H
#define SENSORS_H

#include <Adafruit_BMP085.h>
#include "PinDefinitions.h"

namespace sensors {
float atm_air_quality(int pin = AIR_QUAL_PIN, unsigned long sampletime_ms = 10000);
float UV_intensity(int pin = UV_INTENSITY_PIN);
float light_intensity(int pin = LIGHT_INTENSITY_PIN);
float atm_CO2_ammount(int pin = CO2_AMMOUNT_PIN);
float atm_air_pressure(Adafruit_BMP085 &sensor);
float atm_smoke_ammount(int pin = SMOKE_AMMOUNT_PIN);
float atm_sound_pressure(int pin = SOUND_PRESSURE_PIN);

}  // namespace sensors

#endif  // SENSORS_H
