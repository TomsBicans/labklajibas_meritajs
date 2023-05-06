#include "MonitoringDevice.h"
#include "Measurement.h"

MonitoringDevice::MonitoringDevice() : dht_sensor(DHT_PIN, DHT_TYPE), bmp() {}

void MonitoringDevice::setup() {
    // Call the base class setup function
    BaseDevice::setup();
    // Initialize sensors specific to monitoring device
    Serial.println("Initializing sensors.");
    dht_sensor.begin();
    if (!bmp.begin()) {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    }
    pinMode(NOISE_PIN, INPUT);
    pinMode(AIR_QUAL_PIN, INPUT);
    // Other monitoring device-specific setup
}

void MonitoringDevice::display_device_information() {
    factory_display.clear();
    factory_display.setFont(ArialMT_Plain_10);
    factory_display.drawString(0, 0, "LoRa Node");
    factory_display.drawString(0, 12, "Role: Monitoring Device");
    factory_display.drawString(0, 24, "Sensors: DHT, BMP, Noise, Air Quality");
    factory_display.display();
}

void MonitoringDevice::loop(){
    measurement::entry data_point = measurement::entry{};
    // delay(800);
    // data_point.atm_temperature = dht_sensor.readTemperature();
    // delay(500);
    // data_point.atm_humidity = dht_sensor.readHumidity();
    // delay(500);

    // // ATMOSPHERE AIR PRESSURE
    // delay(800);
    // data_point.atm_air_pressure = sensors::atm_air_pressure(bmp);
    
    // // AIR PARTICLE COUNT
    // delay(800);
    // data_point.atm_air_particle = sensors::atm_air_quality(AIR_QUAL_PIN, 10000);

    // // UV RADIATION
    // delay(800);
    // data_point.UV_intensity = sensors::UV_intensity(10);

    // // LIGHT INTENSITY
    // delay(800);
    // data_point.light_intensity = sensors::light_intensity(10);

    // // CO2 AMMOUNT
    // delay(800);
    // data_point.atm_CO2_ammount = sensors::atm_CO2_ammount(CO2_PWM_PIN);

    // // ATMOSPHERE SMOKE AMMOUNT
    // delay(800);
    // data_point.atm_air_smoke = sensors::atm_smoke_ammount(10);

    // // ATMOSPHERE SOUND PRESSURE
    // delay(800);
    // data_point.atm_sound_pressure = sensors::atm_sound_pressure(NOISE_SENSOR_A_PIN);
    data_point.time = 0;
    // measurement::print_entry(data_point);
}

// Add other monitoring device-specific functionality implementations as needed
