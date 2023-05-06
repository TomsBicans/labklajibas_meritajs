#include "AdministratorDevice.h"

void AdministratorDevice::setup() {
    // Call the base class setup function
    BaseDevice::setup();

    // Initialize inputs specific to administrator device
    pinMode(USER_INPUT_PIN_1_CONST, INPUT);
    pinMode(USER_INPUT_PIN_2_CONST, INPUT);
    // Other administrator device-specific setup
}

void AdministratorDevice::display_device_information() {
    factory_display.clear();
    factory_display.setFont(ArialMT_Plain_10);
    factory_display.drawString(0, 0, "LoRa Node");
    factory_display.drawString(0, 12, "Role: Administrator Device");
    factory_display.drawString(0, 24, "Inputs: User Input 1, User Input 2");
    factory_display.display();
}

void AdministratorDevice::loop(){
  Serial.println("Administrator device loop");
  delay(1000);
}

// Add other administrator device-specific functionality implementations as needed
