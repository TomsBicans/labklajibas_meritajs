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
    static const size_t NUM_SAMPLES = 10;
    ReceiverStats() 
        : totalPacketsReceived(0), totalBytesReceived(0), rssiIndex(0), snrIndex(0), rssiCount(0), snrCount(0) {
            memset(rssiSamples, 0, sizeof(rssiSamples));
            memset(snrSamples, 0, sizeof(snrSamples));
        }

    void packetReceived(uint16_t bytesReceived, int16_t rssi, int8_t snr) {
        this->totalPacketsReceived++;
        this->totalBytesReceived += bytesReceived;
        
        rssiSamples[rssiIndex] = rssi;
        rssiIndex = (rssiIndex + 1) % NUM_SAMPLES;
        rssiCount = min(rssiCount + 1, NUM_SAMPLES);

        snrSamples[snrIndex] = snr;
        snrIndex = (snrIndex + 1) % NUM_SAMPLES;
        snrCount = min(snrCount + 1, NUM_SAMPLES);
    }

    uint32_t getPacketsReceived() const {
        return this->totalPacketsReceived;
    }

    uint32_t getTotalBytesReceived() const {
        return this->totalBytesReceived;
    }

    double getAverageRssi() const {
        double sum = 0;
        size_t count = min(rssiCount, NUM_SAMPLES);
        for(size_t i = 0; i < count; i++) {
            sum += rssiSamples[i];
        }
        return count > 0 ? sum / count : 0;
    }

    double getAverageSnr() const {
        double sum = 0;
        size_t count = min(snrCount, NUM_SAMPLES);
        for(size_t i = 0; i < count; i++) {
            sum += snrSamples[i];
        }
        return count > 0 ? sum / count : 0;
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
    int16_t rssiSamples[NUM_SAMPLES];
    int8_t snrSamples[NUM_SAMPLES];
    size_t rssiIndex;
    size_t snrIndex;
    size_t rssiCount;
    size_t snrCount;
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


#define BUFFER_SIZE                                 256 // Define the payload size here
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
    size_t txpacket_size;
    char rxpacket[BUFFER_SIZE];
    size_t rxpacket_size;
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
