class Timer {
  private:
    unsigned long startTime;  // Timer start time
    unsigned long interval;  // Timer interval

  public:
    // Constructor, takes the timer interval as an argument
    Timer(unsigned long interval) {
      // Initialize the interval and start time
      this->interval = interval;
      startTime = millis();
    }

    // Check if the timer interval has elapsed
    bool hasElapsed() {
      return (millis() - startTime >= interval);
    }

    // Set the timer interval
    void setInterval(unsigned long newInterval) {
      // Update the interval
      interval = newInterval;
    }
    // Reset the timer with a new interval
    void reset(unsigned long newInterval) {
      // Update the interval
      setInterval(newInterval);

      // Update the start time to the current time
      startTime = millis();
    }
};