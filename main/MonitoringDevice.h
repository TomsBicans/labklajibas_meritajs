#ifndef MONITORING_DEVICE_H
#define MONITORING_DEVICE_H

#include "BaseDevice.h"
#include "PinDefinitions.h"
// Include necessary libraries for MonitoringDevice
#include <DHT.h>
#include <Adafruit_BMP085.h>

extern DHT dht_sensor;
extern Adafruit_BMP085 bmp;

void setupMonitoringDevice();
void loopMonitoringDevice();
void displayDeviceInfoMonitoringDevice();

void OnTxDoneMonitoringDevice();
void OnTxTimeoutMonitoringDevice();
void OnRxDoneMonitoringDevice(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);


// class MonitoringDevice : public BaseDevice {
// public:
//     MonitoringDevice();
//     void setup() override;
//     void display_device_information() override;
//     void loop() override;

//     static void OnTxDone();
//     static void OnTxTimeout();
//     static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);
//     // Add other monitoring device-specific functionality as needed

// private:
//     // Add private members for MonitoringDevice as needed
//     DHT dht_sensor;
//     Adafruit_BMP085 bmp;
// };

#endif // MONITORING_DEVICE_H
