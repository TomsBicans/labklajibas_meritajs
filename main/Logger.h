#ifndef LOGGER_H
#define LOGGER_H

#include <Preferences.h>
#include <stdio.h>

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
  SYSTEM_STATUS_3 = 102,

  // Device State
  DEVICE_STATE_FIELD1 = 150,
  // Transmission Stats
  TRANSMISSION_STATS_FIELD1 = 155,
  TRANSMISSION_STATS_FIELD2 = 156,
  TRANSMISSION_STATS_FIELD3 = 157,
  TRANSMISSION_STATS_FIELD4 = 158,
  // Receiver Stats
  RECEIVER_STATS_FIELD1 = 160,
  RECEIVER_STATS_FIELD2 = 161,
  RECEIVER_STATS_FIELD3 = 162,
  RECEIVER_STATS_FIELD4 = 163,
  // Device Information
  DEVICE_INFORMATION_FIELD1 = 165,
  DEVICE_INFORMATION_FIELD2 = 166,

  // Measurement entry
  ENTRY_TIME = 170,
  ENTRY_ATM_TEMPERATURE = 171,
  ENTRY_ATM_HUMIDITY = 172,
  ENTRY_ATM_AIR_PRESSURE = 173,
  ENTRY_ALTITUDE = 174
  // ENTRY_ATM_AIR_PARTICLE = 175,
  // ENTRY_ATM_AIR_SMOKE = 176,
  // ENTRY_ATM_CO2_AMOUNT = 177,
  // ENTRY_ATM_SOUND_PRESSURE = 178,
  // ENTRY_LIGHT_INTENSITY = 179,
  // ENTRY_UV_INTENSITY = 180,
  // ENTRY_QUALITY_RATING = 181,
  // ENTRY_USER_LIKES = 182
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
    char buffer[512]; // Adjust the buffer size according to your needs
    snprintf(buffer, sizeof(buffer),
             "X_ID: %lu, Target Device: %u, Origin Device: %u, Semantic Value: %u, Timestamp: %lu, Numeric Value: %s, Is Float: %s",
             x_id, targetDevice, originDevice, static_cast<uint8_t>(semanticValue), timestamp,
             isFloat ? String(numericValue.floatValue).c_str() : String(numericValue.intValue).c_str(),
             isFloat ? "True" : "False");
    return String(buffer);
  }
};

constexpr const char* LOGGER_NAMESPACE = "logger";
constexpr const char* LOGGER_TEST_NAMESPACE = "loggertest";

// Logger class
class Logger {
  private:
    Preferences preferences;
    uint32_t logCount;
    const char* logCountKey = "logcount";
    const char* namespaceString;
    void write_log(LogEntry entry);

  public:
    Logger(const char* namespaceString);

    void log(uint32_t x_id, uint8_t targetDevice, uint8_t originDevice, SemanticValue semanticValue, float numericValue);
    void log(uint32_t x_id, uint8_t targetDevice, uint8_t originDevice, SemanticValue semanticValue, long int numericValue);
    void clearLogs();
    uint32_t getLogCount();
    LogEntry getLog(uint32_t index);
};

#endif  // LOGGER_H
