#include "LoRaWan_APP.h"
#include "Arduino.h"
// Core functionality
#include "BaseDevice.h"
#include "AdministratorDevice.h"
#include "MonitoringDevice.h"

// TESTS and modules
#include <ArduinoUnit.h>
#include "TestMeasurement.h"
#include "TestTimer.h"
#include "TestQueue.h"
#include "TestLinkedList.h"
#include "TestLogger.h"
#include "Logger.h"


// Constants definitions
static RadioEvents_t RadioEvents;
bool purgeNVS = true;
DeviceRole deviceRole = MONITORING_DEVICE; // Set the device role here

Device assignRoleFunctions(DeviceRole role)
{
    Device device;
    if (role == ADMINISTRATOR)
    {
        Serial.println("Administrator device selected.");
        device.setupFunc = setupAdministrator;
        device.loopFunc = loopAdministrator;
        device.displayDeviceInfoFunc = displayDeviceInfoAdministrator;
        device.OnTxDoneFunc = OnTxDoneAdministrator;
        device.OnTxTimeoutFunc = OnTxTimeoutAdministrator;
        device.OnRxDoneFunc = OnRxDoneAdministrator;
    }
    else if (role == MONITORING_DEVICE)
    {
        Serial.println("Monitoring device selected.");
        device.setupFunc = setupMonitoringDevice;
        device.loopFunc = loopMonitoringDevice;
        device.displayDeviceInfoFunc = displayDeviceInfoMonitoringDevice;
        device.OnTxDoneFunc = OnTxDoneMonitoringDevice;
        device.OnTxTimeoutFunc = OnTxTimeoutMonitoringDevice;
        device.OnRxDoneFunc = OnRxDoneMonitoringDevice;
    }
    return device;
}

Device device = assignRoleFunctions(deviceRole);
void setup()
{
    // Common setup
    setupBase();
    // Setup LoRaWAN
    setupLoRaWAN(
      &RadioEvents,
      device.OnTxDoneFunc,
      device.OnTxTimeoutFunc,
      device.OnRxDoneFunc
    );
    if (deviceRole == LOG_PRINTER){
      Serial.println("Printing logs...");
      uint32_t logCount = logger.getLogCount();
      Serial.print(logCount);
      Serial.println(" log entries found");
        for (uint32_t i = 0; i < logCount; ++i) {
            LogEntry entry = logger.getLog(i);
            Serial.println(String("Log ") + i + ": " + entry.toString());
        }
        if (purgeNVS){
          Serial.println("Deleting all logger storage.");
          logger.clearLogs();
        }
        while (true) delay(10000); // Halt the program
    }
    // Setup for the role
    device.setupFunc();
    device.displayDeviceInfoFunc();
}

void loop()
{
    // Test::run();
    if (device.loopFunc)
    {
        device.loopFunc();
    }
    Radio.IrqProcess();
}