// LCD DISPLEJS
#include <stdio.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <SPI.h>
#include <SD.h>
#include "LCD_config.h"
#include "Messages.h"
#include "Test.h"

// Defines
#define USER_INPUT_PIN_1 3
#define USER_INPUT_PIN_2 5
#define SD_CS_PIN 4
#define NOISE_PIN 6
#define DHT_PIN 2
#define DHT_TYPE DHT22
#define AIR_QUAL_PIN 8
#define BUZZER_PIN 9

//Typedefs
typedef bool (*BoolFunc)();


namespace util{
    static void init_serial(unsigned int baud_rate=9600)
    {
        Serial.begin(baud_rate);
        while (!Serial)
            ;
        delay(300);
        Serial.println("Start");
    }
    int clamp(int n, int lower, int upper)
    {
        return max(lower, min(n, upper));
    }
    char *char_arr(int size)
    {
        size = clamp(size, 0, abs(size));
        char *t = new char[size + 1];
        for (int i=0; i<size; i++){
          t[i] = "\0";
        }
        t[size] = "\0";
        return t;
    }
    long int benchmark(long int t1){
      long int t2 = millis();
      return t2 - t1;
    }

    void benchmark(long int t1, long int t2){
      return t2 - t1;
    }
    LinkedList<int> find_I2C_devices(bool suppress_msg=true){
        // Atgriež sarakstu ar adresēm, kurās atrodas I2C iekārtas.
        byte error, address;
        int device_count;
        int res = 0;
        LinkedList<int> devices;
        if (!suppress_msg) Serial.println("Scanning...");

        device_count = 0;
        for (address = 1; address < 127; address++ ) {
          Wire.begin();
          Wire.beginTransmission(address);
          error = Wire.endTransmission();
          if (error == 0) {
            if (!suppress_msg) Serial.print("I2C device found at address 0x");
            if (address < 16)
              if (!suppress_msg) Serial.print("0");
            if (!suppress_msg) Serial.print(address, HEX);
            devices.add(address);
            if (!suppress_msg) Serial.println("  !");
            res++;
            device_count++;
          }
          else if (error == 4) {
            if (!suppress_msg) Serial.print("Unknown error at address 0x");
            if (address < 16)
              if (!suppress_msg) Serial.print("0");
            if (!suppress_msg) Serial.println(address, HEX);
          }
        }
        if (res == 0){
          if (!suppress_msg) Serial.println("No I2C devices found\n");
          return devices;
        }
        else{
          if (!suppress_msg) Serial.println("done\n");
          return devices;
        }}
    int printLcd(lcd_config config, char text[]) {
      // Return the number of symbols written on the screen.
      config.lcd.clear();
      int numCols = config.columns; // Assume a 16-column LCD screen
      int numRows = config.rows; // Assume a 2-row LCD screen
      int textLen = strlen(text); // Get the length of the text array
      int col = 0, row = 0; // Start at the first column and first row
      int written = 0;
      for (int i = 0; i < textLen; i++) {
        written++;
        if (text[i] == '\n') { // If a newline character is found
          col = 0; // Reset the column to 0
          row++; // Increment the row
          if (row >= numRows) { // If the row is outside the bounds of the screen
            break; // Stop printing
          }
        } else {
          config.lcd.setCursor(col, row); // Set the cursor position
          config.lcd.print(text[i]); // Print the character
          col++; // Increment the column
          if (col >= numCols) { // If the column is outside the bounds of the screen
            col = 0; // Reset the column to 0
            row++; // Increment the row
            if (row >= numRows) { // If the row is outside the bounds of the screen
              break; // Stop printing
            }
          }
        }
      }
      return written;
    }
    char* f2str(float value, char *result) {
      // Convert the floating-point number to a string with two decimal places
      // and a minimum width of 8 characters (including the decimal point and the sign)
      dtostrf(value, 2, 1, result);
      return result;
    }

    int countWords(char *str) {
      // Function that counts the number of words in a char array
      int count = 0;  // Word count

      // Use strtok() to split the char array into individual words
      char *token = strtok(str, " ");
      while (token != NULL) {
        // Increment the word count
        count++;

        // Get the next word in the char array
        token = strtok(NULL, " ");
      }

      // Return the word count
      return count;
    }
    void display_whole_text_LCD(lcd_config config, char text[]){
      // Display the whole text in LCD screen, even if the whole text does not fit on the screen at once.
      int total_length = strlen(text);
      int printed_c = util::printLcd(config, text);
      while (printed_c < total_length){
        Serial.println(printed_c);
        delay(2000);
        printed_c += util::printLcd(config, text+printed_c);
      }
      delay(2000);
    }
    void run_tests(lcd_config config, LinkedList<BoolFunc>& tests){
      int size = tests.getSize();
      int passed = 0;
      char buffer[40];
      for (LinkedList<BoolFunc>::Node* node = tests.getHead(); node != nullptr; node = node->next) {
        bool result = node->data();
        if (result){
            passed++;
            sprintf(buffer, "Tests passed:\n%d/%d", passed, size);
            util::printLcd(config, buffer);
            Serial.println("Test passed.");
        }

      }      
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
    // void read_avg_from_file(File f, )
};


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
    }
  float light_intensity(int pin){
    /*TODO: implement */
    }
  float atm_CO2_ammount(int pin){
    /*TODO: implement */
    }
  float atm_air_pressure(int pin){
    /*TODO: implement */
    }
  float atm_smoke_ammount(int pin){
    /*TODO: implement */
    }
  float atm_sound_pressure(int pin){
    /*TODO: implement */
    }
};

namespace operation {
    enum MODE {
      OBS,
      BENCH
    };
    MODE select_mode(unsigned long time_limit=10*1000, MODE default_mode=OBS){
      MODE mode = default_mode;
      Timer t = Timer(time_limit);
      while(!t.hasElapsed()){
          int tmp_mode = digitalRead(USER_INPUT_PIN_1);
          if (tmp_mode == HIGH){
            mode = OBS;
            break;
          }
          tmp_mode = digitalRead(USER_INPUT_PIN_2);
          if (tmp_mode == HIGH){
            mode = BENCH;
            break;
          }
          delay(20);
      }
      return mode;
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

// GLOBAL VARIABLES
volatile bool NOTIFICATIONS = true;
const bool SD_CARD_LOG = false;

// SCREENS:
LiquidCrystal_I2C lcd1(0x27, 16, 2);
lcd_config lcd1_conf = {lcd1, 16, 2};
// SENSORS
DHT dht_sensor = DHT(DHT_PIN, DHT_TYPE);

void setup()
{
    buzzer::system_on();
    long int start = millis();
    util::init_serial(9600);
    // Find screens
    int count = 0;
    while ((count = util::find_I2C_devices(false).getSize()) < 1){
      Serial.print("Screens found: "); Serial.println(count); delay(2000);
    }
    // Serial.print("SCREEN COUNT: "); Serial.println(count);
    // Initialize screens.
    Serial.println("Initializing screens.");
    lcd1.init(); 
    lcd1.backlight();
    // Initialize sensors.
    Serial.println("Initializing sensors.");
    dht_sensor.begin();
    pinMode(NOISE_PIN, INPUT);
    pinMode(AIR_QUAL_PIN, INPUT);
    pinMode(USER_INPUT_PIN_1, INPUT);

    // Initialize result storage.
    if (SD_CARD_LOG){
      if (!SD.begin(SD_CS_PIN)){
        Serial.println("Failed to initialize SD card storage.");
        return;
      }
      else{
        Serial.println("SD card storage initialized successfully!");
      }
    }
    // Tests
    Serial.println("Running tests.");
    LinkedList<BoolFunc> tests = LinkedList<BoolFunc>();
    tests.add(testQueue);
    tests.add(testLinkedList);
    // tests.add(test_entry_struct);
    // tests.add(test_entry_struct_values);
    // tests.add(test_entry_struct_addition);
    util::run_tests(lcd1_conf,tests);

    // Setup total time.
    long int total = util::benchmark(start);
    print::total_time(total);
    Serial.println("Setup done.");
    // User interaction.
    bool skipintro = true;
    if (!skipintro){
      char TEXT_BUFFER[300];
      Timer timer = Timer(10*1000);
      sprintf(TEXT_BUFFER, MSG_EN_GREET);
      while (!timer.hasElapsed()){ util::display_whole_text_LCD(lcd1_conf, TEXT_BUFFER); continue;}
      timer.reset(10*1000);
      sprintf(TEXT_BUFFER, MSG_EN_INSTRUCTIONS);
      while (!timer.hasElapsed()){ util::display_whole_text_LCD(lcd1_conf, TEXT_BUFFER); continue;}
    }
    buzzer::success();
    delay(1500);
}

int n = 0;
void loop()
{
    buzzer::cycle_start();
    // put your main code here, to run repeatedly:
    long int start = millis();
    char TEXT_BUFFER[40];

    sprintf(TEXT_BUFFER, "Selecting mode\nOBS: B1 BEN: B2");
    util::printLcd(lcd1_conf, TEXT_BUFFER);
    operation::MODE mode = operation::select_mode(2*1000, operation::OBS);
    if (mode == operation::OBS){
      sprintf(TEXT_BUFFER, "Obs mode selected, cycle: %d", n);
      util::printLcd(lcd1_conf, TEXT_BUFFER);
      delay(2000);
      sprintf(TEXT_BUFFER, "Measuring temperature and humidity.");
      util::printLcd(lcd1_conf, TEXT_BUFFER);
      delay(800);
      float temperature = dht_sensor.readTemperature();delay(500);
      float humidity = dht_sensor.readHumidity();delay(500);
      char tmp_temp[10];
      char tmp_hum[10];
      sprintf(TEXT_BUFFER, "Temp: %s (*C)\nHum: %s (%%)", util::f2str(temperature, tmp_temp), util::f2str(humidity, tmp_hum));
      util::display_whole_text_LCD(lcd1_conf, TEXT_BUFFER);
      delay(2000);

      sprintf(TEXT_BUFFER, "Measuring air quality...");
      util::printLcd(lcd1_conf, TEXT_BUFFER);
      float particle_concentration = sensors::atm_air_quality(AIR_QUAL_PIN, 10000);
      char tmp_particle[10];
      sprintf(TEXT_BUFFER, "Conc: %s pcs/L", util::f2str(particle_concentration, tmp_particle));
      util::display_whole_text_LCD(lcd1_conf, TEXT_BUFFER);
      delay(2000);
      // Write data to SD card.
      if (SD_CARD_LOG){
        
        sprintf(TEXT_BUFFER, "Writing data to SD card.");
        util::display_whole_text_LCD(lcd1_conf, TEXT_BUFFER);
        measurement::entry data_entry = measurement::entry{};
        data_entry.atm_temperature = temperature;
        data_entry.atm_humidity = humidity;
        data_entry.atm_air_particle = particle_concentration;
        File log = SD.open("data_meteo.bin", FILE_WRITE);
        if (log){
          //File successfully opened.
          util::write_measurement_to_file(log, data_entry);
        }
        log.close();
      }

    }
    else if (mode == operation::BENCH) {
      sprintf(TEXT_BUFFER, "Benchmark mode selected.");
      delay(2000);
      util::display_whole_text_LCD(lcd1_conf, TEXT_BUFFER);
    }
    delay(1000);
    // Benchmark stats.
    n++;
    long int total = util::benchmark(start);
    Serial.print(n); Serial.print(" ");
    print::total_time(total);
}
