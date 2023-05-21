#include "AdministratorDevice.h"


void setupAdministrator()
{
    // Setup for administrator
    configureRadioForRx();
    test();

    // Initialize inputs specific to administrator device
    // pinMode(USER_INPUT_PIN_1_CONST, INPUT);
    // pinMode(USER_INPUT_PIN_2_CONST, INPUT);
    // Other administrator device-specific setup
}

void loopAdministrator()
{
    // Implement loop logic for administrator
    if (g_deviceState.loraIdle){
      g_deviceState.loraIdle = false;
      Serial.println("into RX mode");
      Radio.Rx(0);
    }
    Radio.IrqProcess();
}

void displayDeviceInfoAdministrator()
{
    // Display device information on the screen.
    g_deviceState.factory_display.clear();
    g_deviceState.factory_display.setFont(ArialMT_Plain_10);
    g_deviceState.factory_display.drawString(0, 0, "LoRa Node");
    g_deviceState.factory_display.drawString(0, 12, "Role: Administrator Device");
    g_deviceState.factory_display.drawString(0, 24, "Inputs: User Input 1, User Input 2");
    g_deviceState.factory_display.display();
}

void OnTxDoneAdministrator()
{
    // Implement OnTxDone logic for administrator
    Serial.println("TX done......");
    g_deviceState.loraIdle = true;
}

void OnTxTimeoutAdministrator()
{
    // Implement OnTxTimeout logic for administrator
    Radio.Sleep( );
    Serial.println("TX Timeout......");
    g_deviceState.loraIdle = true;
}

void OnRxDoneAdministrator(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
    //Implement OnRxDone logic for administrator
    memcpy(g_deviceState.rxpacket, payload, size );
    g_deviceState.rxpacket[size]='\0';
    Radio.Sleep( );
    receiverStats.packetReceived(size, rssi, snr);
    logReceiverStats(&logger, ADMINISTRATOR, ADMINISTRATOR, receiverStats);
    displayReceiverStats(receiverStats);
    g_deviceState.loraIdle = true;

    Serial.print("\r\nreceived packet \"");
    for (size_t i = 0; i < size; i++) {
        Serial.printf("%02X ", (uint8_t)g_deviceState.rxpacket[i]);
    }
    Serial.printf("\" with rssi %d , length %d\r\n", rssi, size);

    // Do data processing.
    LogPacket data = deserializeLogPacket(g_deviceState, size);
    
    // Print the log entries.
    for (uint32_t i = 0; i < data.count; ++i) {
      Serial.println(String("Log ") + i + ": " + data.entries[i].toString());
      logger.write_log(data.entries[i]);
    }
}

void test(){
  Serial.println("Administrator test");
}