#ifndef TIMER_H
#define TIMER_H

class Timer {
  private:
    unsigned long startTime;
    unsigned long interval;

  public:
    Timer(unsigned long interval);

    // Check if the timer interval has elapsed
    bool hasElapsed();

    // Set the timer interval
    void setInterval(unsigned long newInterval);

    // Reset the timer with a new interval
    void reset(unsigned long newInterval);
};

#endif  // TIMER_H
