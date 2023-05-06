#include "Timer.h"
#include <Arduino.h>

Timer::Timer(unsigned long interval) {
  // Initialize the interval and start time
  this->interval = interval;
  startTime = millis();
}

bool Timer::hasElapsed() {
  return (millis() - startTime >= interval);
}

void Timer::setInterval(unsigned long newInterval) {
  // Update the interval
  interval = newInterval;
}

void Timer::reset(unsigned long newInterval) {
  // Update the interval
  setInterval(newInterval);

  // Update the start time to the current time
  startTime = millis();
}
