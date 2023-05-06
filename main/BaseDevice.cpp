#include <WiFi.h> // Include the necessary libraries
#include <Arduino.h>
#include "BaseDevice.h"
#include "util.h" // Include your util library if needed
#include "print.h"

BaseDevice::BaseDevice()
    : factory_display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED),
      ui(&factory_display) {}

void BaseDevice::setup() {
    long int start = millis();
    // Initialize system
    util::init_serial(9600);
    factory_display.init();
    factory_display.setFont(ArialMT_Plain_10);
    WiFi.mode(WIFI_OFF);
    // Setup total time.
    long int total = util::benchmark(start);
    print::total_time(total);
}
void BaseDevice::loop(){
  Serial.println("Base device loop");
  delay(1000);
}
