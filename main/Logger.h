#ifndef LOGGER_H
#define LOGGER_H

#include <Preferences.h>
#include <stdio.h>

constexpr const char* LOGGER_NAMESPACE = "logger";

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
  uint32_t x_id;
  SemanticValue semanticValue;
  unsigned long timestamp;
  NumericValue numericValue;
  bool isFloat;

  String toString() const {
    char buffer[256]; // Adjust the buffer size according to your needs
    snprintf(buffer, sizeof(buffer),
             "X_ID: %lu, Target Device: %u, Origin Device: %u, Semantic Value: %u, Timestamp: %lu, Numeric Value: %s, Is Float: %s",
             x_id, targetDevice, originDevice, static_cast<uint8_t>(semanticValue), timestamp,
             isFloat ? String(numericValue.floatValue).c_str() : String(numericValue.intValue).c_str(),
             isFloat ? "True" : "False");
    return String(buffer);
  }
};

// Logger class
class Logger {
  private:
    Preferences preferences;
    uint32_t logCount;
    const char* logEntryKey = "logentry";
    const char* logCountKey = "logcount";

  public:
    Logger(const char* namespaceString);

    void log(uint32_t x_id, uint8_t targetDevice, uint8_t originDevice, SemanticValue semanticValue, float numericValue);
    void log(uint32_t x_id, uint8_t targetDevice, uint8_t originDevice, SemanticValue semanticValue, long int numericValue);
    void clearLogs();
    uint32_t getLogCount();
    LogEntry getLog(uint32_t index);
};

#endif  // LOGGER_H
