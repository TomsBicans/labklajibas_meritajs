#include "Logger.h"

Logger::Logger(const char* namespaceString): namespaceString(namespaceString) {
  logCount = getLogCount();
}

void Logger::write_log(LogEntry entry){
  preferences.begin(this->namespaceString, false);
  logCount = preferences.getUInt(logCountKey, 0);
  size_t bytesWritten = preferences.putBytes(String(logCount).c_str(), &entry, sizeof(entry));
  if (bytesWritten == 0) {
    Serial.println("Failed to write log entry.");
  } else {
    Serial.print(bytesWritten);
    Serial.println(" bytes written for log entry.");
  }

  size_t op = preferences.putUInt(logCountKey, ++logCount);
  if (op == 0){
    Serial.print("Logcount update not successful.");
  }else{
    Serial.print(op);
    Serial.print(" bytes written.");
    Serial.println("logcount update successful.");
  }
  preferences.end();
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

  write_log(entry);
  delay(20);
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

  write_log();
  delay(20);
}

void Logger::clearLogs() {
  preferences.begin(this->namespaceString, false);
  logCount = preferences.getUInt(logCountKey, 0);
  for (uint32_t i = 0; i < logCount; i++) {
    preferences.remove(String(i).c_str());
  }
  preferences.clear();
  preferences.putUInt(logCountKey, 0);
  logCount = 0;
  preferences.end();
  delay(20);
}

uint32_t Logger::getLogCount() {
  preferences.begin(namespaceString, true);
  logCount = preferences.getUInt(logCountKey, 0);
  preferences.end();
  return logCount;
}

LogEntry Logger::getLog(uint32_t index) {
  LogEntry entry;
  preferences.begin(this->namespaceString, true);
  preferences.getBytes(String(index).c_str(), &entry, sizeof(entry));
  preferences.end();
  return entry;
}
