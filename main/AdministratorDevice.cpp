#include "AdministratorDevice.h"


void setupAdministrator()
{
    // Setup for administrator
    configureRadioForRx();

    // Initialize inputs specific to administrator device
    // pinMode(USER_INPUT_PIN_1_CONST, INPUT);
    // pinMode(USER_INPUT_PIN_2_CONST, INPUT);
    // Other administrator device-specific setup
}

void loopAdministrator()
{
    // Implement loop logic for administrator
    if (loraIdle){
      loraIdle = false;
      Serial.println("into RX mode");
      Radio.Rx(0);
    }
    Radio.IrqProcess();
}

void displayDeviceInfoAdministrator()
{
    // Display device information on the screen.
    factory_display.clear();
    factory_display.setFont(ArialMT_Plain_10);
    factory_display.drawString(0, 0, "LoRa Node");
    factory_display.drawString(0, 12, "Role: Administrator Device");
    factory_display.drawString(0, 24, "Inputs: User Input 1, User Input 2");
    factory_display.display();
}

void OnTxDoneAdministrator()
{
    // Implement OnTxDone logic for administrator
    Serial.println("TX done......");
    loraIdle = true;
}

void OnTxTimeoutAdministrator()
{
    // Implement OnTxTimeout logic for administrator
    Radio.Sleep( );
    Serial.println("TX Timeout......");
    loraIdle = true;
}

void OnRxDoneAdministrator(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
    //Implement OnRxDone logic for administrator
    memcpy(rxpacket, payload, size );
    rxpacket[size]='\0';
    Radio.Sleep( );
    Serial.printf("\r\nreceived packet \"%s\" with rssi %d , length %d\r\n",rxpacket,rssi,size);
    loraIdle = true;
}

// AdministratorDevice::AdministratorDevice() {
//     OnTxDoneFunc = &AdministratorDevice::OnTxDone;
//     OnTxTimeoutFunc = &AdministratorDevice::OnTxTimeout;
//     OnRxDoneFunc = &AdministratorDevice::OnRxDone;
// }

// void AdministratorDevice::display_device_information() {
//     factory_display.clear();
//     factory_display.setFont(ArialMT_Plain_10);
//     factory_display.drawString(0, 0, "LoRa Node");
//     factory_display.drawString(0, 12, "Role: Administrator Device");
//     factory_display.drawString(0, 24, "Inputs: User Input 1, User Input 2");
//     factory_display.display();
// }

// void AdministratorDevice::setup() {
//     // Call the base class setup function
//     BaseDevice::setup();
//     BaseDevice::setupLoRaWAN();
//     BaseDevice::configureRadioForRx();

//     // Initialize inputs specific to administrator device
//     pinMode(USER_INPUT_PIN_1_CONST, INPUT);
//     pinMode(USER_INPUT_PIN_2_CONST, INPUT);
//     // Other administrator device-specific setup
// }

// void AdministratorDevice::loop(){
//   if (loraIdle){
//     loraIdle = false;
//     Serial.println("into RX mode");
//     Radio.Rx(0);
//   }
//   Radio.IrqProcess();
//   // BaseDevice::loopLoRaWAN();
// }

// // Add other administrator device-specific functionality implementations as needed

// void AdministratorDevice::OnTxDone(void) {
//   Serial.println("TX done......");
//   loraIdle = true;
// }

// void AdministratorDevice::OnTxTimeout(void) {
//   Radio.Sleep();
//   Serial.println("TX Timeout......");
//   loraIdle = true;
// }

// void AdministratorDevice::OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
// {
//     memcpy(rxPacket, payload, size );
//     rxPacket[size]='\0';
//     Radio.Sleep( );
//     Serial.printf("\r\nreceived packet \"%s\" with rssi %d , length %d\r\n",rxPacket,rssi,size);
//     loraIdle = true;
// }