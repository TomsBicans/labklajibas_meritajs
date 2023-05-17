#include "Logger.h"

Logger::Logger(const char* namespaceString) {
  preferences.begin(namespaceString, false);
  logCount = preferences.getUInt(logCountKey, 0);
}

void Logger::log(uint32_t x_id, uint8_t targetDevice, uint8_t originDevice, SemanticValue semanticValue, float numericValue) {
  LogEntry entry;
  entry.x_id = x_id;
  entry.targetDevice = targetDevice;
  entry.originDevice = originDevice;
  entry.semanticValue = semanticValue;
  entry.timestamp = millis();
  entry.numericValue.floatValue = numericValue;
  entry.isFloat = true;

  preferences.putBytes(String(logCount).c_str(), &entry, sizeof(entry));
  preferences.putUInt(logCountKey, ++logCount);
}

void Logger::log(uint32_t x_id, uint8_t targetDevice, uint8_t originDevice, SemanticValue semanticValue, long int numericValue) {
  LogEntry entry;
  entry.x_id = x_id;
  entry.targetDevice = targetDevice;
  entry.originDevice = originDevice;
  entry.semanticValue = semanticValue;
  entry.timestamp = millis();
  entry.numericValue.intValue = numericValue;
  entry.isFloat = false;

  preferences.putBytes(String(logCount).c_str(), &entry, sizeof(entry));
  preferences.putUInt(logCountKey, ++logCount);
}

void Logger::clearLogs() {
  for (uint32_t i = 0; i < logCount; i++) {
    preferences.remove(String(i).c_str());
  }
  preferences.putUInt(logCountKey, 0);
  logCount = 0;
}

uint32_t Logger::getLogCount() {
  return logCount;
}

LogEntry Logger::getLog(uint32_t index) {
  LogEntry entry;
  preferences.getBytes(String(index).c_str(), &entry, sizeof(entry));
  return entry;
}
