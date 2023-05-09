#include "LoRaWan_APP.h"
#include "Arduino.h"
#include "AdministratorDevice.h"
#include "MonitoringDevice.h"

// Constants definitions
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

void setupLoRaWAN(
  void (*OnTxDoneFunc)(),
  void (*OnTxTimeoutFunc)(),
  void (*OnRxDoneFunc)(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
);

DeviceRole deviceRole = MONITORING_DEVICE; // Set the device role here
Device device;

// Other variables
void assignRoleFunctions()
{
    if (deviceRole == ADMINISTRATOR)
    {
        Serial.println("Administrator device selected.");
        device.setupFunc = setupAdministrator;
        device.loopFunc = loopAdministrator;
        device.displayDeviceInfoFunc = displayDeviceInfoAdministrator;
        device.OnTxDoneFunc = OnTxDoneAdministrator;
        device.OnTxTimeoutFunc = OnTxTimeoutAdministrator;
        device.OnRxDoneFunc = OnRxDoneAdministrator;
    }
    else if (deviceRole == MONITORING_DEVICE)
    {
        Serial.println("Monitoring device selected.");
        device.setupFunc = setupMonitoringDevice;
        device.loopFunc = loopMonitoringDevice;
        device.displayDeviceInfoFunc = displayDeviceInfoMonitoringDevice;
        device.OnTxDoneFunc = OnTxDoneMonitoringDevice;
        device.OnTxTimeoutFunc = OnTxTimeoutMonitoringDevice;
        device.OnRxDoneFunc = OnRxDoneMonitoringDevice;
    }
}

void setup()
{
    assignRoleFunctions();
    // Common setup
    setupBase();
    // Setup LoRaWAN
    setupLoRaWAN(
      device.OnTxDoneFunc,
      device.OnTxTimeoutFunc,
      device.OnRxDoneFunc);
    // Setup for the role
    device.setupFunc();
    device.displayDeviceInfoFunc();
}

void loop()
{
    if (device.loopFunc)
    {
        device.loopFunc();
    }
}

void setupLoRaWAN(
    void (*OnTxDoneFunc)(),
    void (*OnTxTimeoutFunc)(),
    void (*OnRxDoneFunc)(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
) {
    Serial.println("Initializing LORAWAN setup.");
    Mcu.begin();

    txNumber = 0;
    loraIdle = true;

    RadioEvents.TxDone = OnTxDoneFunc;
    RadioEvents.TxTimeout = OnTxTimeoutFunc;
    RadioEvents.RxDone = OnRxDoneFunc;

    Radio.Init(&RadioEvents);   
    Radio.SetChannel(RF_FREQUENCY);
    Serial.println("LORAWAN setup complete.");
    
    Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                      LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                      LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                      true, 0, 0, LORA_IQ_INVERSION_ON, 3000);
    Serial.println("Changed radio mode to transmitter");
}

