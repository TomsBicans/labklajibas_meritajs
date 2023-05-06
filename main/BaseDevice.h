// BaseDevice.h
#ifndef BASE_DEVICE_H
#define BASE_DEVICE_H

#include "HT_SSD1306Wire.h"
#include "HT_DisplayUi.h"

class BaseDevice {
public:
    SSD1306Wire factory_display;
    DisplayUi ui;

    BaseDevice();
    virtual void setup() = 0;
    virtual void display_device_information() = 0;
    virtual void loop() = 0;
    // Add other common functionality as needed
};

#endif // BASE_DEVICE_H
