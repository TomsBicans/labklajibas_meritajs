#include "Logger.h"

Logger::Logger(const char* namespaceString, const char* dataFileName): namespaceString(namespaceString), dataFileName(dataFileName) {
  logCount = getLogCount();
  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
}

void Logger::write_log(LogEntry entry){
  preferences.begin(this->namespaceString, false);
  logCount = preferences.getUInt(logCountKey, 0);
  File file = SPIFFS.open(dataFileName, "a");
    if (!file) {
      Serial.println("Failed to open log file for writing.");
    } else {
      size_t bytesWritten = file.write((uint8_t*)&entry, sizeof(entry));
      if (bytesWritten == 0) {
        Serial.println("Failed to write log entry.");
      } else {
        Serial.print(bytesWritten);
        Serial.println(" bytes written for log entry.");
        logCount++;
      }
      file.close();
    }
  size_t op = preferences.putUInt(logCountKey, logCount);
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

  write_log(entry);
}

void Logger::clearLogs() {
  // Remove the data file
  SPIFFS.remove(dataFileName);
  // Reset the log counter.
  preferences.begin(this->namespaceString, false);
  preferences.clear();
  preferences.putUInt(logCountKey, 0);
  logCount = 0;
  preferences.end();
}

uint32_t Logger::getLogCount() {
  preferences.begin(namespaceString, true);
  logCount = preferences.getUInt(logCountKey, 0);
  preferences.end();
  return logCount;
}

LogEntry Logger::getLog(uint32_t index) {
  LogEntry entry;
  File file = SPIFFS.open(dataFileName, "r");
    if (file) {
      file.seek(index * sizeof(LogEntry), SeekSet);
      file.read((uint8_t*)&entry, sizeof(entry));
      file.close();
    }
  return entry;
}

void Logger::printLogs(){
  Serial.println("Printing logs...");
  uint32_t logCount = getLogCount();
  Serial.print(logCount);
  Serial.println(" log entries found");
  
  File file = SPIFFS.open(dataFileName, "r");
  if (!file) {
      Serial.println("Failed to open log file for reading.");
      return;
  }

  LogEntry entry;
  for (uint32_t i = 0; i < logCount; ++i) {
      if (file.read((uint8_t*)&entry, sizeof(entry)) == sizeof(entry)) {
          Serial.println(String("Log ") + i + ": " + entry.toString());
      } else {
          Serial.println("Failed to read log entry.");
          break;
      }
  }
  file.close();
}
