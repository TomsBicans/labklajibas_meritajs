#include <Arduino.h>
#include <WiFi.h>

#include "BaseDevice.h"
#include "util.h"
#include "print.h"

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];
double txNumber;
bool loraIdle = true;
// RadioEvents_t RadioEvents;

SSD1306Wire factory_display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED);
DisplayUi ui(&factory_display);
// SSD1306Wire factory_display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED);
// DisplayUi ui(&factory_display);


void setupBase(){
    long int start = millis();
    // Initialize system
    util::init_serial(115200);
    factory_display.init();
    factory_display.setFont(ArialMT_Plain_10);
    WiFi.mode(WIFI_OFF);
    // Setup total time.
    long int total = util::benchmark(start);
    print::total_time(total);
    delay(300);
}

// void setupLoRaWAN(
//     void (*OnTxDoneFunc)(),
//     void (*OnTxTimeoutFunc)(),
//     void (*OnRxDoneFunc)(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
// ) {
//     Serial.println("Initializing LORAWAN setup.");
//     Mcu.begin();

//     txNumber = 0;
//     loraIdle = true;

//     RadioEvents.TxDone = OnTxDoneFunc;
//     RadioEvents.TxTimeout = OnTxTimeoutFunc;
//     RadioEvents.RxDone = OnRxDoneFunc;

//     Radio.Init(&RadioEvents);
    
//     Serial.println("LORAWAN setup complete.");
//     configureRadioForTx();
// }


void configureRadioForTx(){
    Radio.SetChannel(RF_FREQUENCY);
    Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                      LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                      LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                      true, 0, 0, LORA_IQ_INVERSION_ON, 3000);
    Serial.println("Changed radio mode to transmitter");
}

void configureRadioForRx(){
    Radio.SetChannel(RF_FREQUENCY);
    Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                      LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                      LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                      0, true, 0, 0, LORA_IQ_INVERSION_ON, true);
    Serial.println("Changed radio mode to receiver");
}


// char BaseDevice::txPacket[BUFFER_SIZE];
// char BaseDevice::rxPacket[BUFFER_SIZE];
// double BaseDevice::txNumber = 0;
// bool BaseDevice::loraIdle = true;


// BaseDevice::BaseDevice()
//     : factory_display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED),
//       ui(&factory_display) {
//       }

// void BaseDevice::setup() {
//     long int start = millis();
//     // Initialize system
//     util::init_serial(115200);
//     factory_display.init();
//     factory_display.setFont(ArialMT_Plain_10);
//     WiFi.mode(WIFI_OFF);
//     // Setup total time.
//     long int total = util::benchmark(start);
//     print::total_time(total);
//     delay(300);
// }

// void BaseDevice::loop(){
//   Serial.println("Base device loop");
//   delay(1000);
// }

// void BaseDevice::setupLoRaWAN() {
//     Serial.println("Initializing LORAWAN setup.");
//     Mcu.begin();

//     txNumber = 0;
//     loraIdle = true;
//     Serial.print("OnTxDoneFunc address: ");
//     Serial.println((uintptr_t)OnTxDoneFunc, HEX);
//     Serial.print("OnTxTimeoutFunc address: ");
//     Serial.println((uintptr_t)OnTxTimeoutFunc, HEX);
//     Serial.print("OnRxDoneFunc address: ");
//     Serial.println((uintptr_t)OnRxDoneFunc, HEX);


//     radioEvents.TxDone = OnTxDoneFunc;
//     radioEvents.TxTimeout = OnTxTimeoutFunc;
//     radioEvents.RxDone = OnRxDoneFunc;

//     Serial.print("radioEvents.TxDone address: ");
//     Serial.println((uintptr_t)radioEvents.TxDone, HEX);
//     Serial.print("radioEvents.TxTimeout address: ");
//     Serial.println((uintptr_t)radioEvents.TxTimeout, HEX);
//     Serial.print("radioEvents.RxDone address: ");
//     Serial.println((uintptr_t)radioEvents.RxDone, HEX);

//     Radio.Init(&radioEvents);
//     Radio.SetChannel(RF_FREQUENCY);
//     Serial.println("LORAWAN setup complete.");
// }

// void BaseDevice::configureRadioForTx() {
//     Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
//                       LORA_SPREADING_FACTOR, LORA_CODINGRATE,
//                       LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
//                       true, 0, 0, LORA_IQ_INVERSION_ON, 3000);
//     Serial.println("Changed radio mode to transmitter");
// }

// void BaseDevice::configureRadioForRx() {
//     Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
//                       LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
//                       LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
//                       0, true, 0, 0, LORA_IQ_INVERSION_ON, true);
//     Serial.println("Changed radio mode to receiver");
// }

// void BaseDevice::sendLoRaMessage(const char* message) {
//     if (loraIdle) {
//         Serial.printf("\r\nsending packet \"%s\" , length %d\r\n", message, strlen(message));
//         Radio.Send((uint8_t *)message, strlen(message));
//         loraIdle = false;
//     }
// }

// void BaseDevice::loopLoRaWAN(){
//   Radio.IrqProcess();
// }