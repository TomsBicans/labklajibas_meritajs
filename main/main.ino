// LCD DISPLEJS
#include <stdio.h>
#include <Wire.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <SPI.h>
#include <SD.h>
#include "Messages.h"
#include "DS3231.h"
#include "HT_SSD1306Wire.h"
#include "HT_DisplayUi.h"
#include "WiFi.h"

// TESTS and custom made modules
#include <ArduinoUnit.h>
#include "TestMeasurement.h"
#include "TestTimer.h"
#include "TestQueue.h"
#include "TestLinkedList.h"

// Defines
// for I2C sensors
#define USER_INPUT_PIN_1 3
#define USER_INPUT_PIN_2 5
#define SD_CS_PIN 49
#define NOISE_PIN 6
#define DHT_PIN 2
#define DHT_TYPE DHT22
#define AIR_QUAL_PIN 8
#define BUZZER_PIN 9
#define CO2_PWM_PIN 10
#define NOISE_SENSOR_A_PIN 0

// SYSTEM TYPEs
#define MONITORING_DEVICE 1
#define ADMINISTRATOR_DEVICE 2

// Selected system role.
#define DEVICE_TYPE ADMINISTRATOR_DEVICE // Change this to ADMINISTRATOR_DEVICE for the administrator device

// GLOBAL VARIABLES
const bool SD_CARD_LOG = true;
SSD1306Wire  factory_display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED); // addr , freq , i2c group , resolution , rst
DisplayUi ui(&factory_display);

// SENSORS
DHT dht_sensor = DHT(DHT_PIN, DHT_TYPE);
Adafruit_BMP085 bmp;

namespace util{
    static void init_serial(unsigned int baud_rate=9600)
    {
        Serial.begin(baud_rate);
        while (!Serial)
            ;
        delay(300);
        Serial.println("Start");
    }
    // static void init_lora(long frequency){
    //   const int maxRetries = 10; // Set the maximum number of retries
    //   int retryCount = 0;

    //   while (!LoRa.begin(frequency)) {
    //     retryCount++;
    //     if (retryCount > maxRetries) {
    //       Serial.println("Starting LoRa failed after multiple attempts!");
    //       break; // Exit the loop if the maximum number of retries is reached
    //     }
    //     Serial.print("Attempt ");
    //     Serial.print(retryCount);
    //     Serial.println(": Starting LoRa failed, retrying...");
    //     delay(1000); // Add a delay between each attempt
    //   }
    // }
    int clamp(int n, int lower, int upper)
    {
        return max(lower, min(n, upper));
    }
    unsigned long benchmark(unsigned long t1){
      unsigned long t2 = millis();
      return t2 - t1;
    }

    long int benchmark(long int t1, long int t2){
      return t2 - t1;
    }
    char* f2str(float value, char *result) {
      // Convert the floating-point number to a string with two decimal places
      // and a minimum width of 8 characters (including the decimal point and the sign)
      dtostrf(value, 2, 1, result);
      return result;
    }

    bool float_compare(float x, float y, float tolerance) {
      if (abs(x - y) < tolerance) {
        return true;
      } else {
        return false;
      }
    }
    void write_measurement_to_file(File file, measurement::entry data){
      file.write((byte*)&data, sizeof(data));
    }

    void read_struct_from_file(File file, measurement::entry &data) {
      file.read((byte*)&data, sizeof(data));
    }

    measurement::entry read_avg_struct_from_file(File file, measurement::entry &data){
      int count = 0;
      measurement::entry res = measurement::entry{};
      while(file.available()){
        measurement::entry tmp = measurement::entry{};
        file.read((byte*)&tmp, sizeof(tmp));
        res = res + tmp;
        count += 1;
      }
      res = res / count;
      return res;
    }

};

namespace oled_screen{
  void displayLoopInfo(SSD1306Wire &display, int loopCount, unsigned long globalTime, unsigned long lastLoopTime) {
    display.clear();
    display.setFont(ArialMT_Plain_10);

    display.drawString(0, 0, "Loop Info:");
    display.drawString(0, 12, "Loop Count: " + String(loopCount));
    display.drawString(0, 24, "Global Time: " + String(globalTime) + " ms");
    display.drawString(0, 36, "Last Loop Time: " + String(lastLoopTime) + " ms");

    display.display();
  }

  void display_device_information() {
    factory_display.clear();
    factory_display.setFont(ArialMT_Plain_10);
    factory_display.drawString(0, 0, "LoRa Node");
    
    #if DEVICE_TYPE == MONITORING_DEVICE
        factory_display.drawString(0, 12, "Role: Monitoring Device");
        factory_display.drawString(0, 24, "Sensors: DHT, BMP, Noise, Air Quality");
    #elif DEVICE_TYPE == ADMINISTRATOR_DEVICE
        factory_display.drawString(0, 12, "Role: Administrator Device");
        factory_display.drawString(0, 24, "Inputs: User Input 1, User Input 2");
    #endif
    
    factory_display.display();
}

}

namespace print{
    void total_time(long int dif){
      Serial.print("Total loop time: "); Serial.print(dif); Serial.println(" milliseconds");
    }
};

namespace sensors{
  float atm_air_quality(int pin=AIR_QUAL_PIN, unsigned long sampletime_ms=10000){
    //https://www.howmuchsnow.com/arduino/airquality/grovedust/
    unsigned long duration;
    unsigned long starttime;
    unsigned long lowpulseoccupancy = 0;
    float ratio = 0;
    float concentration = 0;
    Timer t = Timer(sampletime_ms);
    while(!t.hasElapsed()){
      duration = pulseIn(pin, LOW);
      lowpulseoccupancy += duration;
    }
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62;
    if (concentration <= 0.62){
      return 0;
    }
    concentration *= 100; // Convert pcs/0.01cf => pcs/cf
    concentration /= 0.0283168466; // Convert pcs/cf => pcs/m^3
    concentration /= 1000; // Convert pcs/m^3 => pcs/L
    return concentration;
    }
  float UV_intensity(int pin){
    /*TODO: implement */
    return 0;
    }
  float light_intensity(int pin){
    /*TODO: implement */
    return 0;
    }
  float atm_CO2_ammount(int pin){
    /*TODO: implement */
    return 0;
    float res = 0;
    float count = 0;
    for (int i=0; i<4; i++){
      unsigned long sample_time = 1004; // ms
      float time_high = 2;
      float time_low = 2;
      unsigned long duration;
      float result;    
      Timer t = Timer(sample_time);
      while (!t.hasElapsed()){
        duration = pulseIn(pin, LOW);
        time_low += duration;
      }
      time_high = sample_time - time_low - 2;
      // time_low = sample_time - time_high - 2;
      result = 5000*(time_high-2)/(time_high+time_low-4);
      res += result;
      count += 1;
    }
    return res/count;
    }
  float atm_air_pressure(Adafruit_BMP085 &sensor){
    /*TODO: implement */
    float res = sensor.readPressure();
    res = res / 1000; // Pa -> kPa
    return res;
    }
  float atm_smoke_ammount(int pin){
    /*TODO: implement */
    return 0;
    }
  float atm_sound_pressure(int pin){
    float res = 0;
    int count = 0;
    Timer t = Timer(3*1000);
    while(!t.hasElapsed()){
      res += analogRead(pin);
      count++;
      delay(50);
    }
    res /= count;
    return res;
    }
};

namespace operation {
    enum BUTTON {
      BTN1,
      BTN2
    };
    BUTTON button_press(unsigned long time_limit=10*1000, BUTTON default_mode=BTN1){
      BUTTON btn = default_mode;
      Timer t = Timer(time_limit);
      while(!t.hasElapsed()){
          int tmp_mode = digitalRead(USER_INPUT_PIN_1);
          if (tmp_mode == HIGH){
            btn = BTN1;
            break;
          }
          tmp_mode = digitalRead(USER_INPUT_PIN_2);
          if (tmp_mode == HIGH){
            btn = BTN2;
            break;
          }
          delay(20);
      }
      return btn;
    }
}

namespace buzzer {
  const int BUZZ_PIN = BUZZER_PIN;
  const unsigned int plank_buzz = 10;
  const unsigned int short_buzz = 100;
  const unsigned int med_buzz = 400;
  const unsigned int long_buzz = 800;
  namespace note {
      const float C4 = 261.63;
      const float D4 = 293.66;
      const float E4 = 329.63;
      const float F4 = 349.23;
      const float G4 = 392.00;
      const float A4 = 440.00;
      const float B4 = 493.88;
  }
    
  
  void pulse_buzzer(int numPulses, unsigned int delay_ms, unsigned int freq) {
    // Loop through the number of pulses
    for (int i = 0; i < numPulses; i++) {
      // Output the pulse tone on the buzzer pin
      tone(BUZZ_PIN, freq);
      delay(delay_ms);
      // Turn off the buzzer
      noTone(BUZZ_PIN);
      delay(delay_ms);
    }
  }
  void buzz(unsigned int time, unsigned int freq){
    tone(BUZZ_PIN, freq);
    delay(time);
    noTone(BUZZ_PIN);
  }
  void system_on(){
    buzz(short_buzz, note::C4);
    delay(short_buzz);
    buzz(short_buzz, note::E4);
    delay(short_buzz);
    buzz(short_buzz, note::C4*2);
  }
  void cycle_start(){
    buzz(plank_buzz, note::A4);
  }
  // void cycle_end(){}
  void error(){
    buzz(long_buzz, note::C4/(2^2));
  }
  void success(){
    pulse_buzzer(2, short_buzz, note::A4);
  }
  void warning(){
    pulse_buzzer(2, med_buzz, note::C4/2);
  }
  void saving_data(){
    buzz(short_buzz, note::E4);    
  }
}

void setup()
{
    long int start = millis();
    // Initialize system
    util::init_serial(9600);
    factory_display.init();
    factory_display.setFont(ArialMT_Plain_10);
    WiFi.mode(WIFI_OFF);
    factory_display.drawString(0, 0, "LoRa Node");
    factory_display.display();

    // Device role
    #if DEVICE_TYPE == MONITORING_DEVICE
      // Initialize sensors.
      Serial.println("Initializing sensors.");
      dht_sensor.begin();
      if (!bmp.begin()) {
          Serial.println("Could not find a valid BMP085 sensor, check wiring!");
      }
      pinMode(NOISE_PIN, INPUT);
      pinMode(AIR_QUAL_PIN, INPUT);

    #elif DEVICE_TYPE == ADMINISTRATOR_DEVICE
      pinMode(USER_INPUT_PIN_1, INPUT);
      pinMode(USER_INPUT_PIN_2, INPUT);
    #endif

    oled_screen::display_device_information();

    // Setup total time.
    long int total = util::benchmark(start);
    print::total_time(total);
}

int LOOP_COUNT = 0;
unsigned long GLOBAL_TIME = 0;
void loop()
{
  Test::run();
  buzzer::cycle_start();
  long int start = millis();
  #if DEVICE_TYPE == MONITORING_DEVICE
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
    data_point.time = GLOBAL_TIME;
    // measurement::print_entry(data_point);
  #elif DEVICE_TYPE == ADMINISTRATOR_DEVICE

  #endif

  delay(2000);

  LOOP_COUNT++;
  unsigned long total = util::benchmark(start);
  GLOBAL_TIME += (unsigned long)abs((int)total);
  Serial.print(LOOP_COUNT); Serial.print(" ");
  print::total_time(total);
  // oled_screen::displayLoopInfo(factory_display, LOOP_COUNT, GLOBAL_TIME, total);
}