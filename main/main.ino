// LCD DISPLEJS
#include <stdio.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <SPI.h>
#include <SD.h>
#include "DS3231.h"
#include "HT_SSD1306Wire.h"
#include "HT_DisplayUi.h"
#include "WiFi.h"

// TESTS and custom made modules
#include <ArduinoUnit.h>
#include "Messages.h"
#include "TestMeasurement.h"
#include "TestTimer.h"
#include "TestQueue.h"
#include "TestLinkedList.h"
#include "BaseDevice.h"
#include "MonitoringDevice.h"
#include "AdministratorDevice.h"
#include "util.h"
#include "print.h"
#include "Buzzer.h"
#include "Sensors.h"
#include "Operation.h"

// Sensor pin constants.
#include "PinDefinitions.h"


// SYSTEM TYPEs
#define MONITORING_DEVICE 1
#define ADMINISTRATOR_DEVICE 2

// Selected system role.
#define DEVICE_TYPE MONITORING_DEVICE // Change this to ADMINISTRATOR_DEVICE for the administrator device

BaseDevice *device = nullptr;

BaseDevice* initDevice(int role) {
    switch (role) {
        case MONITORING_DEVICE:
            return new MonitoringDevice();
        case ADMINISTRATOR_DEVICE:
            return new AdministratorDevice();
        default:
            return nullptr;
    }
}

void setup() {
    // Create the appropriate device instance based on the role
    device = initDevice(DEVICE_TYPE);

    // Call the setup function for the device role
    if (device != nullptr) {
        device->setup();
        device->display_device_information();
    } else {
        Serial.println("Invalid device role!");
    }
}

unsigned long loopCount = 0;
unsigned long globalTime = 0;

void loop() {
  // Test::run();
  buzzer::cycle_start();

  unsigned long startTime = millis();

  if (device != nullptr) {
    device->loop();
  } else {
    Serial.println("Invalid device role!");
    delay(2000);
  }

  loopCount++;
  unsigned long elapsedTime = util::benchmark(startTime);
  globalTime += elapsedTime;

  // Serial.print(loopCount);
  // Serial.print(" ");
  // print::total_time(elapsedTime);
}