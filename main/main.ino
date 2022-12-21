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

    void read_struct_from_file(File file, measurement::entry &data) {
      file.read((byte*)&data, sizeof(data));
      // file.read((char*)&data, sizeof(data));
    }

    void read_avg_struct_from_file(File file, measurement::entry &data){
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
    }
  float atm_air_pressure(int pin){
    /*TODO: implement */
    return 0;
    }
  float atm_smoke_ammount(int pin){
    /*TODO: implement */
    return 0;
    }
  float atm_sound_pressure(int pin){
    /*TODO: implement */
    return 0;
    }
};

namespace rating{
    enum QUALITY {
      VL = 75,
      L = 80,
      M = 90,
      H = 100,
    };
    
  QUALITY temperature(float value){
    return H;
  }
  QUALITY humidity(float value){
    return H;
  }  
  QUALITY atm_air_quality(float value){
    return H;
  }
  QUALITY UV_intensity(float value){
    /*TODO: implement */
    return H;
    }
  QUALITY light_intensity(float value){
    /*TODO: implement */
    return H;
    }
  QUALITY atm_CO2_ammount(float value){
    /*TODO: implement */
    return H;
    }
  QUALITY atm_air_pressure(float value){
    /*TODO: implement */
    return H;
    }
  QUALITY atm_smoke_ammount(float value){
    /*TODO: implement */
    return H;
    }
  QUALITY atm_sound_pressure(float value){
    /*TODO: implement */
    return H;
    }
};

namespace operation {
    enum MODE {
      OBS,
      BENCH
    };
    enum BUTTON {
      BTN1,
      BTN2
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
    int measure_count = 1;
    if (mode == operation::OBS){
      measure_count = 1;
      sprintf(TEXT_BUFFER, "Obs mode selected, cycle: %d", n);
      util::printLcd(lcd1_conf, TEXT_BUFFER);
      delay(2000);
    }
    else if (mode == operation::BENCH) {      
      measure_count = 3;
      sprintf(TEXT_BUFFER, "Benchmark mode selected.");
      util::display_whole_text_LCD(lcd1_conf, TEXT_BUFFER);
      delay(2000);
    }
    measurement::entry result = measurement::entry{};
    for (int i=0; i<measure_count; i++){
        measurement::entry data_point = measurement::entry{};
        // AIR TEMPERATURE AND HUMIDITY
        sprintf(TEXT_BUFFER, "Measuring temperature and humidity.");
        util::printLcd(lcd1_conf, TEXT_BUFFER);
        delay(800);
        data_point.atm_temperature = dht_sensor.readTemperature();delay(500);
        data_point.atm_humidity = dht_sensor.readHumidity();delay(500);
        char tmp_temp[10];
        char tmp_hum[10];
        sprintf(TEXT_BUFFER, "Temp: %s (*C)\nHum: %s (%%)", util::f2str(data_point.atm_temperature, tmp_temp), util::f2str(data_point.atm_humidity, tmp_hum));
        util::display_whole_text_LCD(lcd1_conf, TEXT_BUFFER);
        delay(1000);

        // AIR PARTICLE COUNT
        sprintf(TEXT_BUFFER, "Measuring air quality...");
        delay(800);
        util::printLcd(lcd1_conf, TEXT_BUFFER);
        data_point.atm_air_particle = sensors::atm_air_quality(AIR_QUAL_PIN, 10000);
        sprintf(TEXT_BUFFER, "Conc: %s pcs/L", util::f2str(data_point.atm_air_particle, tmp_temp));
        util::display_whole_text_LCD(lcd1_conf, TEXT_BUFFER);
        delay(1000);

        // UV RADIATION
        sprintf(TEXT_BUFFER, "Measuring UV radiation...");
        delay(800);
        util::printLcd(lcd1_conf, TEXT_BUFFER);
        data_point.UV_intensity = sensors::UV_intensity(10);
        sprintf(TEXT_BUFFER, "UV radiation:\n%s ev", util::f2str(data_point.UV_intensity, tmp_temp));
        util::printLcd(lcd1_conf, TEXT_BUFFER);
        delay(1000);

        // LIGHT INTENSITY
        sprintf(TEXT_BUFFER, "Measuring light intensity...");
        delay(800);
        util::printLcd(lcd1_conf, TEXT_BUFFER);
        data_point.light_intensity = sensors::light_intensity(10);
        sprintf(TEXT_BUFFER, "Light: %s lux", util::f2str(data_point.light_intensity, tmp_temp));
        util::printLcd(lcd1_conf, TEXT_BUFFER);
        delay(1000);

        // CO2 AMMOUNT
        sprintf(TEXT_BUFFER, "Measuring CO2 ammount..");
        delay(800);
        util::printLcd(lcd1_conf, TEXT_BUFFER);
        data_point.atm_CO2_ammount = sensors::atm_CO2_ammount(10);
        sprintf(TEXT_BUFFER, "CO2: %s ppm", util::f2str(data_point.atm_CO2_ammount, tmp_temp));
        util::printLcd(lcd1_conf, TEXT_BUFFER);
        delay(1000);

        // ATMOSPHERE AIR PRESSURE
        sprintf(TEXT_BUFFER, "Measuring air pressure..");
        delay(800);
        util::printLcd(lcd1_conf, TEXT_BUFFER);
        data_point.atm_air_pressure = sensors::atm_air_pressure(10);
        sprintf(TEXT_BUFFER, "Air pressure:\n%s pascal", util::f2str(data_point.atm_air_pressure, tmp_temp));
        util::printLcd(lcd1_conf, TEXT_BUFFER);
        delay(1000);

        // ATMOSPHERE SMOKE AMMOUNT
        sprintf(TEXT_BUFFER, "Measuring smoke ammount..");
        delay(800);
        util::printLcd(lcd1_conf, TEXT_BUFFER);
        data_point.atm_air_smoke = sensors::atm_smoke_ammount(10);
        sprintf(TEXT_BUFFER, "Smoke:\n%s ppm", util::f2str(data_point.atm_air_smoke, tmp_temp));
        util::printLcd(lcd1_conf, TEXT_BUFFER);
        delay(1000);

        // ATMOSPHERE SOUND PRESSURE
        sprintf(TEXT_BUFFER, "Measuring smoke ammount..");
        delay(800);
        util::printLcd(lcd1_conf, TEXT_BUFFER);
        data_point.atm_sound_pressure = sensors::atm_sound_pressure(10);
        sprintf(TEXT_BUFFER, "Sound pressure:\n%s dB", util::f2str(data_point.atm_sound_pressure, tmp_temp));
        util::printLcd(lcd1_conf, TEXT_BUFFER);
        delay(1000);
        result = result + data_point;
    }
    result = result / measure_count;

    if (mode == operation::BENCH){
      float total_rating = 100;
      total_rating *= rating::temperature(result.atm_temperature);
      total_rating *= rating::humidity(result.atm_humidity);
      total_rating *= rating::atm_air_quality(result.atm_air_particle);
      total_rating *= rating::UV_intensity(result.UV_intensity);
      total_rating *= rating::light_intensity(result.light_intensity);
      total_rating *= rating::atm_CO2_ammount(result.atm_CO2_ammount);
      total_rating *= rating::atm_air_pressure(result.atm_air_pressure);
      total_rating *= rating::atm_smoke_ammount(result.atm_air_smoke);
      total_rating *= rating::atm_sound_pressure(result.atm_sound_pressure);
      char tmp_buff[10];
      sprintf(TEXT_BUFFER, "Total rating:\n%s (0-100)", util::f2str(total_rating, tmp_buff));
      util::printLcd(lcd1_conf, TEXT_BUFFER);
      result.quality_rating = total_rating;
      delay(2000);
      // Do some more user interaction.
      sprintf(TEXT_BUFFER, "Do you like the environment?");
      util::printLcd(lcd1_conf, TEXT_BUFFER);
      operation::BUTTON user_selection = operation::button_press(10*1000, operation::BUTTON::BTN1);
      if (user_selection == operation::BTN1){
        // The user likes.
        result.user_likes = measurement::LIKES::YES;
      }
      if (user_selection == operation::BTN2){
        // The user does not like.
        result.user_likes = measurement::LIKES::NO;
      }
      // Do you want to know more?
      sprintf(TEXT_BUFFER, "Do you want the details?");
      util::printLcd(lcd1_conf, TEXT_BUFFER);
      user_selection = operation::button_press(10*1000, operation::BUTTON::BTN2);
      if (user_selection == operation::BTN1){
        // The user wants to know more.
        // Display each measurement description.
      }
    };

    // Write data to SD card.
    if (SD_CARD_LOG){
      sprintf(TEXT_BUFFER, "Writing data to SD card.");
      util::display_whole_text_LCD(lcd1_conf, TEXT_BUFFER);
      // Write to the end of the file.
      if (mode == operation::OBS){
        File log = SD.open("data_meteo.bin", FILE_WRITE);
        if (log){
          //File successfully opened.
          util::write_measurement_to_file(log, result);
        }
        log.close();
      }
      if (mode == operation::BENCH){
        char f_name[20];       
        if (result.user_likes == measurement::LIKES::YES){
          sprintf(f_name, "bench_likes.bin");
        }
        if (result.user_likes == measurement::LIKES::NO){
          sprintf(f_name, "bench_dislikes.bin");
        }
        File log = SD.open(f_name, FILE_WRITE);
        if (log){
          util::write_measurement_to_file(log, result);
        }
        log.close();
      }
    }

    delay(1000);
    // Benchmark stats.
    n++;
    long int total = util::benchmark(start);
    Serial.print(n); Serial.print(" ");
    print::total_time(total);
}
