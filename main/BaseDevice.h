// BaseDevice.h
#ifndef BASE_DEVICE_H
#define BASE_DEVICE_H

#include "HT_SSD1306Wire.h"
#include "HT_DisplayUi.h"
#include "LoRaWan_APP.h"

#define RF_FREQUENCY                                915000000 // Hz
#define TX_OUTPUT_POWER                             5        // dBm
#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false

#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 30 // Define the payload size here

extern char txpacket[BUFFER_SIZE];
extern char rxpacket[BUFFER_SIZE];
extern double txNumber;
extern bool loraIdle;
// extern RadioEvents_t RadioEvents;

extern SSD1306Wire factory_display;
extern DisplayUi ui;

enum DeviceRole
{
    ADMINISTRATOR,
    MONITORING_DEVICE
};

struct Device
{
    void (*setupFunc)();
    void (*loopFunc)();
    void (*OnTxDoneFunc)();
    void (*OnTxTimeoutFunc)();
    void (*OnRxDoneFunc)(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);
    void (*displayDeviceInfoFunc)();
};

void setupBase();
void configureRadioForTx();
void configureRadioForRx();
void setupLoRaWAN(
  RadioEvents_t *RadioEvents,
  void (*OnTxDoneFunc)(),
  void (*OnTxTimeoutFunc)(),
  void (*OnRxDoneFunc)(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
);
#endif // BASE_DEVICE_H
