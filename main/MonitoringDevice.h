#ifndef MONITORING_DEVICE_H
#define MONITORING_DEVICE_H

#include "BaseDevice.h"
#include "PinDefinitions.h"
// Include necessary libraries for MonitoringDevice
#include <DHT.h>
#include <Adafruit_BMP085.h>

class MonitoringDevice : public BaseDevice {
public:
    MonitoringDevice();
    void setup() override;
    void display_device_information() override;
    void loop() override;
    // Add other monitoring device-specific functionality as needed

private:
    // Add private members for MonitoringDevice as needed
    DHT dht_sensor;
    Adafruit_BMP085 bmp;
};

#endif // MONITORING_DEVICE_H
