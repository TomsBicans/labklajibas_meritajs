#include "radio/radio.h"
#include <Arduino.h>
#include <WiFi.h>

#include "BaseDevice.h"


DeviceState g_deviceState;
TransmissionStats transmissionStats;
ReceiverStats receiverStats;
DeviceInformation deviceInformation;
Logger logger(LOGGER_NAMESPACE, "/logs.bin");

void setupBase(){
    long int start = millis();
    // Initialize system
    util::init_serial(115200);
    g_deviceState.factory_display.init();
    g_deviceState.factory_display.setFont(ArialMT_Plain_10);
    WiFi.mode(WIFI_OFF);
    // Setup total time.
    long int total = util::benchmark(start);
    print::total_time(total);
    delay(300);
}

void setupLoRaWAN(
    RadioEvents_t *RadioEvents,
    void (*OnTxDoneFunc)(),
    void (*OnTxTimeoutFunc)(),
    void (*OnRxDoneFunc)(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
) {
    Serial.println("Initializing LORAWAN setup.");
    Mcu.begin();

    g_deviceState.txNumber = 0;
    g_deviceState.loraIdle = true;

    RadioEvents->TxDone = OnTxDoneFunc;
    RadioEvents->TxTimeout = OnTxTimeoutFunc;
    RadioEvents->RxDone = OnRxDoneFunc;

    Radio.Init(RadioEvents);
    
    Serial.println("LORAWAN setup complete.");
    configureRadioForTx();
}


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

void displayTransmissionStats(const TransmissionStats &st){
  char stats[64];
  g_deviceState.factory_display.clear();
  g_deviceState.factory_display.setFont(ArialMT_Plain_10);
  sprintf(stats, "Packets Sent: %lu", st.getPacketsSent());
  g_deviceState.factory_display.drawString(0, 0, stats);
  sprintf(stats, "Total Bytes Sent: %lu", st.getTotalBytesSent());
  g_deviceState.factory_display.drawString(0, 12, stats);
  sprintf(stats, "On Air Time: %lu ms", st.getTotalOnAirTime());
  g_deviceState.factory_display.drawString(0, 24, stats);
  sprintf(stats, "Avg. Cast Time: %lu ms", st.getAveragePacketSendingTime());
  g_deviceState.factory_display.drawString(0, 36, stats);
  g_deviceState.factory_display.display();
}

void displayReceiverStats(const ReceiverStats &st){
    char stats[64];
    g_deviceState.factory_display.clear();
    g_deviceState.factory_display.setFont(ArialMT_Plain_10);
    sprintf(stats, "Packets Received: %lu", st.getPacketsReceived());
    g_deviceState.factory_display.drawString(0, 0, stats);
    sprintf(stats, "Total Bytes Received: %lu", st.getTotalBytesReceived());
    g_deviceState.factory_display.drawString(0, 12, stats);
    sprintf(stats, "Average RSSI: %.2f", st.getAverageRssi());
    g_deviceState.factory_display.drawString(0, 24, stats);
    sprintf(stats, "Average SNR: %.2f", st.getAverageSnr());
    g_deviceState.factory_display.drawString(0, 36, stats);
    g_deviceState.factory_display.display();
}

LogPacket logDeviceState(Logger *logger, DeviceRole target, DeviceRole origin, const DeviceState state) {
    LogPacket logData;
    uint32_t x_id = millis();
    logData.entries[0] = logger->log(x_id, target, origin, DEVICE_STATE_FIELD1, static_cast<long int>(state.loraIdle));
    logData.count = 1;
    return logData;
}

LogPacket logTransmissionStats(Logger *logger, DeviceRole target, DeviceRole origin, const TransmissionStats stats) {
  LogPacket logData;
  uint32_t x_id = millis();
  logData.entries[0] = logger->log(x_id, target, origin, TRANSMISSION_STATS_FIELD1, static_cast<long int>(stats.getPacketsSent()));
  logData.entries[1] = logger->log(x_id, target, origin, TRANSMISSION_STATS_FIELD2, static_cast<long int>(stats.getAveragePacketSendingTime()));
  logData.entries[2] = logger->log(x_id, target, origin, TRANSMISSION_STATS_FIELD3, static_cast<long int>(stats.getTotalBytesSent()));
  logData.entries[3] = logger->log(x_id, target, origin, TRANSMISSION_STATS_FIELD4, static_cast<long int>(stats.getTotalOnAirTime()));
  logData.count = 4;
  Serial.println("Wrote stats to logger.");
  return logData;
}

LogPacket logReceiverStats(Logger *logger, DeviceRole target, DeviceRole origin, const ReceiverStats stats) {
  LogPacket logData;
  uint32_t x_id = millis();
  logData.entries[0] = logger->log(x_id, target, origin, RECEIVER_STATS_FIELD1, static_cast<long int>(stats.getPacketsReceived()));
  logData.entries[1] = logger->log(x_id, target, origin, RECEIVER_STATS_FIELD2, static_cast<long int>(stats.getTotalBytesReceived()));
  logData.entries[2] = logger->log(x_id, target, origin, RECEIVER_STATS_FIELD3, static_cast<float>(stats.getAverageRssi()));
  logData.entries[3] = logger->log(x_id, target, origin, RECEIVER_STATS_FIELD4, static_cast<float>(stats.getAverageSnr()));
  logData.count = 4;
  return logData;
}

LogPacket logDeviceInformation(Logger *logger, DeviceRole target, DeviceRole origin, const DeviceInformation info) {
  LogPacket logData;
  uint32_t x_id = millis();
  logData.entries[0] = logger->log(x_id, target, origin, DEVICE_INFORMATION_FIELD1, static_cast<long int>(info.getTotalDeviceRuntime()));
  logData.entries[1] = logger->log(x_id, target, origin, DEVICE_INFORMATION_FIELD2, info.getBatteryLevel());
  logData.count = 2;
  return logData;
}

LogPacket logSensorReadings(Logger *logger, DeviceRole target, DeviceRole origin, const measurement::entry entry){
  LogPacket logData;
  uint32_t x_id = millis();
  logData.entries[0] = logger->log(x_id, target, origin, ENTRY_ATM_TEMPERATURE, entry.atm_temperature);
  logData.entries[1] = logger->log(x_id, target, origin, ENTRY_ATM_HUMIDITY, entry.atm_humidity);
  logData.entries[2] = logger->log(x_id, target, origin, ENTRY_ATM_AIR_PRESSURE, entry.atm_air_pressure);
  logData.entries[3] = logger->log(x_id, target, origin, ENTRY_ALTITUDE, entry.atm_altitude);
  logData.count = 4;
  Serial.println("Logged sensor readings.");
  return logData;
  // logger->log(x_id, target, origin, SENSOR_READINGS_FIELD5, entry.atm_air_particle);
  // logger->log(x_id, target, origin, SENSOR_READINGS_FIELD6, entry.atm_air_smoke);
  // logger->log(x_id, target, origin, SENSOR_READINGS_FIELD7, entry.atm_CO2_ammount);
  // logger->log(x_id, target, origin, SENSOR_READINGS_FIELD8, entry.atm_sound_pressure);
  // logger->log(x_id, target, origin, SENSOR_READINGS_FIELD9, entry.light_intensity);
  // logger->log(x_id, target, origin, SENSOR_READINGS_FIELD10, entry.UV_intensity);
  // logger->log(x_id, target, origin, SENSOR_READINGS_FIELD11, entry.quality_rating);
  // logger->log(x_id, target, origin, SENSOR_READINGS_FIELD12, static_cast<long int>(entry.user_likes));
}
