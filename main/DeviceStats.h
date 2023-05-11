// DeviceStats.h
#ifndef DEVICE_STATS_H
#define DEVICE_STATS_H

#include <Arduino.h>

class TransmissionStats {
public:
    TransmissionStats() 
        : totalPacketsSent(0), totalBytesSent(0), totalOnAirTime(0), startTime(0) {}
    
    void startTransmission() {
        this->startTime = millis();
    }
    
    void endTransmission(uint16_t bytesSent) {
        unsigned long endTime = millis();
        this->totalPacketsSent++;
        this->totalBytesSent += bytesSent;
        this->totalOnAirTime += (endTime - this->startTime);
    }

    uint32_t getPacketsSent() const {
        return this->totalPacketsSent;
    }

    uint32_t getTotalBytesSent() const {
        return this->totalBytesSent;
    }

    unsigned long getTotalOnAirTime() const {
        return this->totalOnAirTime;
    }

    unsigned long getAveragePacketSendingTime() const {
        return this->totalPacketsSent > 0 ? this->totalOnAirTime / this->totalPacketsSent : 0;
    }

    void printStats() const {
        Serial.println("Transmission Stats:");
        Serial.print("Total Packets Sent: ");
        Serial.println(this->getPacketsSent());
        Serial.print("Total Bytes Sent: ");
        Serial.println(this->getTotalBytesSent());
        Serial.print("Total On Air Time: ");
        Serial.println(this->getTotalOnAirTime());
        Serial.print("Average Packet Sending Time: ");
        Serial.println(this->getAveragePacketSendingTime());
    }
    
private:
    uint32_t totalPacketsSent;
    uint32_t totalBytesSent;
    unsigned long totalOnAirTime;
    unsigned long startTime;
};


class ReceiverStats {
public:
    ReceiverStats() 
        : totalPacketsReceived(0), totalBytesReceived(0), totalRssi(0), totalSnr(0) {}

    void packetReceived(uint16_t bytesReceived, int16_t rssi, int8_t snr) {
        this->totalPacketsReceived++;
        this->totalBytesReceived += bytesReceived;
        this->totalRssi += rssi;
        this->totalSnr += snr;
    }

    uint32_t getPacketsReceived() const {
        return this->totalPacketsReceived;
    }

    uint32_t getTotalBytesReceived() const {
        return this->totalBytesReceived;
    }

    double getAverageRssi() const {
        return this->totalPacketsReceived > 0 ? (double)this->totalRssi / this->totalPacketsReceived : 0;
    }

    double getAverageSnr() const {
        return this->totalPacketsReceived > 0 ? (double)this->totalSnr / this->totalPacketsReceived : 0;
    }

    void printStats() const {
        Serial.println("Reception Stats:");
        Serial.print("Total Packets Received: ");
        Serial.println(this->getPacketsReceived());
        Serial.print("Total Bytes Received: ");
        Serial.println(this->getTotalBytesReceived());
        Serial.print("Average RSSI: ");
        Serial.println(this->getAverageRssi());
        Serial.print("Average SNR: ");
        Serial.println(this->getAverageSnr());
    }

private:
    uint32_t totalPacketsReceived;
    uint32_t totalBytesReceived;
    int32_t totalRssi;
    int32_t totalSnr;
};

class DeviceInformation {
public:
    DeviceInformation() 
        : totalDeviceRuntime(0), batteryLevel(0), startTime(millis()) {}

    void updateRuntime() {
        this->totalDeviceRuntime = millis() - this->startTime;
    }

    void setBatteryLevel(float level) {
        this->batteryLevel = level;
    }

    unsigned long getTotalDeviceRuntime() const {
        return this->totalDeviceRuntime;
    }

    float getBatteryLevel() const {
        return this->batteryLevel;
    }

    void printInfo() const {
        Serial.println("Device Information:");
        Serial.print("Total Device Runtime: ");
        Serial.println(this->getTotalDeviceRuntime());
        Serial.print("Battery Level: ");
        Serial.println(this->getBatteryLevel());
    }

private:
    unsigned long totalDeviceRuntime;
    float batteryLevel;
    unsigned long startTime;
};


#define BUFFER_SIZE                                 50 // Define the payload size here
class DeviceState {
public:
    DeviceState()
        : txNumber(0), 
          loraIdle(true), 
          factory_display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED),
          ui(&factory_display) {
        memset(txpacket, 0, BUFFER_SIZE);
        memset(rxpacket, 0, BUFFER_SIZE);
    }

    char txpacket[BUFFER_SIZE];
    char rxpacket[BUFFER_SIZE];
    double txNumber;
    bool loraIdle;

    SSD1306Wire factory_display;
    DisplayUi ui;
};

extern DeviceState g_deviceState;
extern DeviceInformation deviceInformation;
extern ReceiverStats receiverStats;
extern TransmissionStats transmissionStats;

#endif // DEVICE_STATS_H
