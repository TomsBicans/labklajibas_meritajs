#include <ArduinoUnit.h>
#include "Logger.h"

test(logging_float_value) {
  Logger logger;
  logger.clearLogs();

  uint8_t targetDevice = 1;
  uint8_t originDevice = 2;
  SemanticValue semanticValue = SENSOR_HUMIDITY;
  float numericValue = 3.14;
  
  logger.log(targetDevice, originDevice, semanticValue, numericValue);

  assertEqual(logger.getLogCount(), 1);

  LogEntry entry = logger.getLog(0);
  assertEqual(entry.targetDevice, targetDevice);
  assertEqual(entry.originDevice, originDevice);
  assertEqual(entry.semanticValue, semanticValue);
  assertEqual(entry.numericValue.floatValue, numericValue);
  assertTrue(entry.isFloat);
  logger.clearLogs();
}

test(logging_int_value) {
  Logger logger;
  logger.clearLogs();

  uint8_t targetDevice = 1;
  uint8_t originDevice = 2;  // added origin device
  SemanticValue semanticValue = ERROR_1;
  long int numericValue = 1234;
  
  logger.log(targetDevice, originDevice, semanticValue, numericValue);

  assertEqual(logger.getLogCount(), 1);

  LogEntry entry = logger.getLog(0);
  assertEqual(entry.targetDevice, targetDevice);
  assertEqual(entry.originDevice, originDevice);  // assert origin device
  assertEqual(entry.semanticValue, semanticValue);
  assertEqual(entry.numericValue.intValue, numericValue);
  assertFalse(entry.isFloat);
  logger.clearLogs();
}

test(clear_logs) {
  Logger logger;
  logger.clearLogs();

  logger.log(1, 1, SENSOR_HUMIDITY, static_cast<float>(3.14));
  logger.log(1, 2, ERROR_1, static_cast<long int>(1234));

  assertEqual(logger.getLogCount(), 2);

  logger.clearLogs();

  assertEqual(logger.getLogCount(), 0);
}


test(multiple_entries) {
  Logger logger;
  logger.clearLogs();

  logger.log(1, 1, SENSOR_HUMIDITY, static_cast<float>(3.14));
  logger.log(1, 2, ERROR_1, static_cast<long int>(1234));
  logger.log(2, 3, SENSOR_TEMPERATURE, static_cast<float>(6.28));
  logger.log(2, 3, ERROR_2, static_cast<long int>(5678));

  assertEqual(logger.getLogCount(), 4);
  logger.clearLogs();
}



test(check_entries) {
  Logger logger;
  logger.clearLogs();

  logger.log(1, 2, SENSOR_HUMIDITY, static_cast<float>(3.14));
  logger.log(1, 3, ERROR_1, static_cast<long int>(1234));

  LogEntry entry1 = logger.getLog(0);
  assertEqual(entry1.targetDevice, 1);
  assertEqual(entry1.originDevice, 2);
  assertEqual(entry1.semanticValue, SENSOR_HUMIDITY);
  assertTrue(entry1.isFloat);
  assertEqual(entry1.numericValue.floatValue, 3.14f);

  LogEntry entry2 = logger.getLog(1);
  assertEqual(entry2.targetDevice, 1);
  assertEqual(entry2.originDevice, 3);
  assertEqual(entry2.semanticValue, ERROR_1);
  assertFalse(entry2.isFloat);
  assertEqual(entry2.numericValue.intValue, 1234);

  logger.clearLogs();
}

test(iterate_entries) {
  Logger logger;
  logger.clearLogs();

  // Log 10 entries.
  for (uint8_t i = 0; i < 10; i++) {
    if (i % 2 == 0) {
      logger.log(i, i + 1, SENSOR_HUMIDITY, static_cast<float>(i * 0.1f));
    } else {
      logger.log(i, i + 1, ERROR_1, static_cast<long int>(i * 100));
    }
  }

  // Check that 10 entries were logged.
  assertEqual(logger.getLogCount(), 10);

  // Iterate over all entries and check their content.
  for (uint8_t i = 0; i < 10; i++) {
    LogEntry entry = logger.getLog(i);
    
    // Check common parts of the entry.
    assertEqual(entry.targetDevice, i);
    assertEqual(entry.originDevice, i + 1);

    // Check the semantic value, which depends on whether i is even or odd.
    if (i % 2 == 0) {
      assertEqual(entry.semanticValue, SENSOR_HUMIDITY);
    } else {
      assertEqual(entry.semanticValue, ERROR_1);
    }

    // Check the numeric value, which depends on whether i is even or odd.
    if (i % 2 == 0) {
      assertTrue(entry.isFloat);
      assertEqual(entry.numericValue.floatValue, i * 0.1f);
    } else {
      assertFalse(entry.isFloat);
      assertEqual(entry.numericValue.intValue, i * 100);
    }
  }
  logger.clearLogs();
}
