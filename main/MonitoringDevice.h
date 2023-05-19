#ifndef MONITORING_DEVICE_H
#define MONITORING_DEVICE_H

#include "BaseDevice.h"
#include "PinDefinitions.h"
#include "Measurement.h"
#include "Logger.h"
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

// Helper functions
#endif // MONITORING_DEVICE_H
