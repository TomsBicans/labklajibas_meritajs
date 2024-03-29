// BaseDevice.h
#ifndef BASE_DEVICE_H
#define BASE_DEVICE_H

#include "HT_SSD1306Wire.h"
#include "HT_DisplayUi.h"
#include "LoRaWan_APP.h"
#include "DeviceStats.h"
#include "Logger.h"
#include <Wire.h>

#include "util.h"
#include "print.h"
#include "Measurement.h"

#define RF_FREQUENCY                                868000000 // Hz
#define TX_OUTPUT_POWER                             14        // dBm
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


extern DeviceState g_deviceState;
extern DeviceInformation deviceInformation;
extern ReceiverStats receiverStats;
extern TransmissionStats transmissionStats;
extern Logger logger;

enum DeviceRole
{
    ADMINISTRATOR,
    MONITORING_DEVICE,
    LOG_PRINTER
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

// Utility functions
void displayTransmissionStats(const TransmissionStats &st);
void displayReceiverStats(const ReceiverStats &st);
void displayDeviceInfoLogger();

// Logger functions
LogPacket logDeviceState(      Logger *logger, DeviceRole target, DeviceRole origin, const DeviceState state);
LogPacket logTransmissionStats(Logger *logger, DeviceRole target, DeviceRole origin, const TransmissionStats stats);
LogPacket logReceiverStats(    Logger *logger, DeviceRole target, DeviceRole origin, const ReceiverStats stats);
LogPacket logDeviceInformation(Logger *logger, DeviceRole target, DeviceRole origin, const DeviceInformation info);
LogPacket logSensorReadings(   Logger *logger, DeviceRole target, DeviceRole origin, const measurement::entry entry);

void serializeLogPacket(DeviceState& state, LogPacket logPacket);
LogPacket deserializeLogPacket(DeviceState& state, size_t bufferSize);

#endif // BASE_DEVICE_H
