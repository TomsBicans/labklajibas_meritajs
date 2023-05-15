#include <Preferences.h>

enum SemanticValue : uint8_t {
  // Sensor readings
  SENSOR_HUMIDITY = 1,
  SENSOR_TEMPERATURE = 2,
  SENSOR_AIR_PRESSURE = 3,
  SENSOR_ALTITUDE = 4,
  // Errors
  ERROR_1 = 50,
  ERROR_2 = 51,
  ERROR_3 = 52,
  ERROR_4 = 53,
  ERROR_5 = 54,

  // System status messages
  SYSTEM_STATUS_1 = 100,
  SYSTEM_STATUS_2 = 101,
  SYSTEM_STATUS_3 = 102
};

// Log entry structure
union NumericValue {
  long int intValue;
  float floatValue;
};

struct LogEntry {
  uint8_t targetDevice;
  uint8_t originDevice;
  SemanticValue semanticValue;
  unsigned long timestamp;
  NumericValue numericValue;
  bool isFloat;
};

// Logger class
class Logger {
  private:
    Preferences preferences;
    uint32_t logCount;
    const char* logEntryKey = "logentry";
    const char* logCountKey = "logcount";

  public:
    Logger();

    void log(uint8_t targetDevice, uint8_t originDevice, SemanticValue semanticValue, float numericValue);
    void log(uint8_t targetDevice, uint8_t originDevice, SemanticValue semanticValue, long int numericValue);
    void clearLogs();
    uint32_t getLogCount();
    LogEntry getLog(uint32_t index);
};
