// AdministratorDevice.h
#ifndef ADMINISTRATOR_DEVICE_H
#define ADMINISTRATOR_DEVICE_H

#include "BaseDevice.h"
#include "Arduino.h"
#include "PinDefinitions.h"

const int USER_INPUT_PIN_1_CONST = USER_INPUT_PIN_1;
const int USER_INPUT_PIN_2_CONST = USER_INPUT_PIN_2;

void setupAdministrator();
void loopAdministrator();

void displayDeviceInfoAdministrator();

void OnTxDoneAdministrator();
void OnTxTimeoutAdministrator();
void OnRxDoneAdministrator(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);

// Helper functions
static void test();

#endif // ADMINISTRATOR_DEVICE_H