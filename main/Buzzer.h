#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>
#include "PinDefinitions.h"

namespace buzzer {
  const int BUZZ_PIN = BUZZER_PIN;
  const unsigned int plank_buzz = 10;
  const unsigned int short_buzz = 100;
  const unsigned int med_buzz = 400;
  const unsigned int long_buzz = 800;

  namespace note {
    const float C4 = 261.63;
    const float D4 = 293.66;
    const float E4 = 329.63;
    const float F4 = 349.23;
    const float G4 = 392.00;
    const float A4 = 440.00;
    const float B4 = 493.88;
  }

  void pulse_buzzer(int numPulses, unsigned int delay_ms, unsigned int freq);
  void buzz(unsigned int time, unsigned int freq);
  void system_on();
  void cycle_start();
  void error();
  void success();
  void warning();
  void saving_data();
}

#endif // BUZZER_H
