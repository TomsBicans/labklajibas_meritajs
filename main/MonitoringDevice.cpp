#include "MonitoringDevice.h"

DHT dht_sensor(DHT_PIN, DHT_TYPE);
// DHTesp dht_sensor;
Adafruit_BMP085 bmp;


void setupMonitoringDevice(){
    configureRadioForTx();

    // Initialize sensors specific to monitoring device
    Serial.println("Initializing sensors.");
    // dht_sensor.setup(DHT_PIN, DHTesp::DHT22);
    dht_sensor.begin();
    if (!bmp.begin()) {
      Serial.println("Could not find a valid BMP085 sensor, check wiring!");
      // while (1);
    }else{
      Serial.println("Found a valid BMP085 sensor");
    }
}


// float temperature = 15;
// float humidity = 80;
// float air_pressure = 1000;
// float altitude = 15;

void loopMonitoringDevice(){
    // Implement loop logic for monitoring device
    if(g_deviceState.loraIdle == true)
    {
      delay(2000);
      // g_deviceState.txNumber += 0.01;
      measurement::entry entry;
      // entry.atm_temperature = dht_sensor.getTemperature();
      entry.atm_temperature = dht_sensor.readTemperature();
      delay(250);
      entry.atm_air_pressure = sensors::atm_air_pressure(bmp);
      delay(200);
      entry.atm_humidity = dht_sensor.readHumidity();
      delay(250);
      entry.atm_altitude = sensors::atm_altitude(bmp);
      delay(200);
      // temperature+= 0.1;
      // humidity+=0.01;

      Serial.print("air pressure (kPa): ");
      Serial.println(entry.atm_air_pressure);

      Serial.print("humidity (%): ");
      Serial.println(entry.atm_humidity);

      Serial.print("temperature (*C):");
      Serial.println(entry.atm_temperature);

      Serial.print("Altitude (m): ");
      Serial.println(entry.atm_altitude);

      LogPacket txdata = logSensorReadings(&logger, ADMINISTRATOR, MONITORING_DEVICE, entry);
      serializeLogPacket(g_deviceState, txdata);

      // size_t packetSize = txdata.count * sizeof(LogEntry);
      
      Serial.print("\r\nsending packet \"");
      for (size_t i = 0; i < g_deviceState.txpacket_size; i++) {
        Serial.printf("%02X ", (uint8_t)g_deviceState.txpacket[i]);
      }
      Serial.printf("\" , length %d\r\n", g_deviceState.txpacket_size);

      transmissionStats.startTransmission();
      Radio.Send( (uint8_t *)g_deviceState.txpacket, g_deviceState.txpacket_size ); //send the package out	
      g_deviceState.loraIdle = false;
    }
}

void displayDeviceInfoMonitoringDevice(){
    g_deviceState.factory_display.clear();
    g_deviceState.factory_display.setFont(ArialMT_Plain_10);
    g_deviceState.factory_display.drawString(0, 0, "Role: Monitoring Device");
    g_deviceState.factory_display.display();
}

void OnTxDoneMonitoringDevice(){
    // Implement OnTxDone logic for monitoring device
    Serial.println("TX done......");
    g_deviceState.loraIdle = true;

    //End the transmission and update the status.
    transmissionStats.endTransmission(g_deviceState.txpacket_size);
    logTransmissionStats(&logger, ADMINISTRATOR, MONITORING_DEVICE, transmissionStats);
    displayTransmissionStats(transmissionStats);
}

void OnTxTimeoutMonitoringDevice(){
    // Implement OnTxTimeout logic for monitoring device
    Radio.Sleep( );
    Serial.println("TX Timeout......");
    g_deviceState.loraIdle = true;
}

void OnRxDoneMonitoringDevice(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr){
    //Implement OnRxDone logic for administrator
    memcpy(g_deviceState.rxpacket, payload, size );
    g_deviceState.rxpacket[size]='\0';
    Radio.Sleep( );
    Serial.printf("\r\nreceived packet \"%s\" with rssi %d , length %d\r\n",g_deviceState.rxpacket,rssi,size);
    receiverStats.packetReceived(size, rssi, snr);
    displayReceiverStats(receiverStats);
    g_deviceState.loraIdle = true;
}


// void MonitoringDevice::loop(){
//   measurement::entry data_point = measurement::entry{};
  
//   if(loraIdle == true){
//     delay(1000);
// 		txNumber += 0.01;
// 		sprintf(txPacket,"Hello world number %0.2f",txNumber);  //start a package
   
// 		Serial.printf("\r\nsending packet \"%s\" , length %d\r\n",txPacket, strlen(txPacket));

// 		Radio.Send( (uint8_t *)txPacket, strlen(txPacket) ); //send the package out	
//     loraIdle = false;
//   }  
//     data_point.time = 0;
//     Radio.IrqProcess();
//     // BaseDevice::loopLoRaWAN();
//     // delay(800);
//     // data_point.atm_temperature = dht_sensor.readTemperature();
//     // delay(500);
//     // data_point.atm_humidity = dht_sensor.readHumidity();
//     // delay(500);

//     // // ATMOSPHERE AIR PRESSURE
//     // delay(800);
//     // data_point.atm_air_pressure = sensors::atm_air_pressure(bmp);
    
//     // // AIR PARTICLE COUNT
//     // delay(800);
//     // data_point.atm_air_particle = sensors::atm_air_quality(AIR_QUAL_PIN, 10000);

//     // // UV RADIATION
//     // delay(800);
//     // data_point.UV_intensity = sensors::UV_intensity(10);

//     // // LIGHT INTENSITY
//     // delay(800);
//     // data_point.light_intensity = sensors::light_intensity(10);

//     // // CO2 AMMOUNT
//     // delay(800);
//     // data_point.atm_CO2_ammount = sensors::atm_CO2_ammount(CO2_PWM_PIN);

//     // // ATMOSPHERE SMOKE AMMOUNT
//     // delay(800);
//     // data_point.atm_air_smoke = sensors::atm_smoke_ammount(10);

//     // // ATMOSPHERE SOUND PRESSURE
//     // delay(800);
//     // data_point.atm_sound_pressure = sensors::atm_sound_pressure(NOISE_SENSOR_A_PIN);
// }