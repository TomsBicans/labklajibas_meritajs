// AdministratorDevice.h
#ifndef ADMINISTRATOR_DEVICE_H
#define ADMINISTRATOR_DEVICE_H

#include "BaseDevice.h"
#include "PinDefinitions.h"


class AdministratorDevice : public BaseDevice {
public:
    void setup() override;
    void display_device_information() override;
    void loop() override;
    // Add other administrator device-specific functionality as needed

private:
    // Add private members for AdministratorDevice as needed
    const int USER_INPUT_PIN_1_CONST = USER_INPUT_PIN_1;
    const int USER_INPUT_PIN_2_CONST = USER_INPUT_PIN_2;
};

#endif // ADMINISTRATOR_DEVICE_H
