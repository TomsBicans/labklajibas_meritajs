#include "Buzzer.h"

namespace buzzer {
  void pulse_buzzer(int numPulses, unsigned int delay_ms, unsigned int freq) {
    // Loop through the number of pulses
    for (int i = 0; i < numPulses; i++) {
      // Output the pulse tone on the buzzer pin
      tone(BUZZ_PIN, freq);
      delay(delay_ms);
      // Turn off the buzzer
      noTone(BUZZ_PIN);
      delay(delay_ms);
    }
  }
  void buzz(unsigned int time, unsigned int freq){
    tone(BUZZ_PIN, freq);
    delay(time);
    noTone(BUZZ_PIN);
  }
  void system_on(){
    buzz(short_buzz, note::C4);
    delay(short_buzz);
    buzz(short_buzz, note::E4);
    delay(short_buzz);
    buzz(short_buzz, note::C4*2);
  }
  void cycle_start(){
    buzz(plank_buzz, note::A4);
  }
  // void cycle_end(){}
  void error(){
    buzz(long_buzz, note::C4/(2^2));
  }
  void success(){
    pulse_buzzer(2, short_buzz, note::A4);
  }
  void warning(){
    pulse_buzzer(2, med_buzz, note::C4/2);
  }
  void saving_data(){
    buzz(short_buzz, note::E4);    
  }
}