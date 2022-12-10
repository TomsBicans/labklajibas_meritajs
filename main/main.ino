// LCD DISPLEJS
#include <stdio.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include "LinkedList.h"
#include "LCD_config.h"
#include "Timer.h"
#include "Messages.h"

// Defines
#define DHT_PIN 7
#define DHT_TYPE DHT22
#define NOISE_PIN 6


namespace util{
    static void init_serial()
    {
        Serial.begin(9600);
        while (!Serial)
            ;
        delay(300);
        Serial.print("Start.\n");
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
        // if (text[i] == ' ') {
        //   // Calculate the length of the next word
        //   int nextWordLength = 0;
        //   for (int j = i + 1; j < textLen; j++) {
        //     if (text[j] == ' ') {
        //       break;
        //     }
        //     nextWordLength++;
        //   }

        //   // Check if the next word would fit on the current line
        //   if (col + nextWordLength >= config.columns) {
        //     // Move to the next line
        //     col = 0;
        //     row++;
        //     if (row >= numRows){
        //       // written--;
        //       break;              
        //     }
        //     i++;
        //     config.lcd.setCursor(col, row);
        //   }
        // }

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
};


namespace print{
    void total_time(long int dif){
      Serial.print("Total loop time: "); Serial.print(dif); Serial.println(" milliseconds");
    }
};

namespace sensors{
  
};

// GLOBAL VARIABLES
// SCREENS:
LiquidCrystal_I2C lcd1(0x27, 16, 2);
lcd_config lcd1_conf = {lcd1, 16, 2};
// SENSORS
DHT dht_sensor = DHT(DHT_PIN, DHT_TYPE);
//
void setup()
{
    long int start = millis();
    util::init_serial();
    // Find screens
    int count = 0;
    while ((count = util::find_I2C_devices(false).getSize()) < 1){
      Serial.print("Screens found: "); Serial.println(count); delay(2000);
    }
    Serial.print("SCREEN COUNT: "); Serial.println(count);
    // Initialize screens.
    lcd1.init(); lcd1.backlight();
    // Initialize sensors.
    dht_sensor.begin();
    pinMode(NOISE_PIN, INPUT);
    // Initialize result storage.

    // Setup total time.
    long int total = util::benchmark(start);
    print::total_time(total);
    // User interaction.
    bool skipintro = true;
    if (!skipintro){
      char buffer[300];
      Timer timer = Timer(10*1000);
      sprintf(buffer, MSG_EN_GREET);
      while (!timer.hasElapsed()){ util::display_whole_text_LCD(lcd1_conf, buffer); continue;}
      timer.reset(10*1000);
      sprintf(buffer, MSG_EN_INSTRUCTIONS);
      while (!timer.hasElapsed()){ util::display_whole_text_LCD(lcd1_conf, buffer); continue;}
    }
}

int n = 0;
void loop()
{
    // put your main code here, to run repeatedly:
    long int start = millis();
    // LinkedList<int> devices = util::find_I2C_devices();
    char buffer[300];
    // sprintf(buffer, "Labdien. Sis ir labklajibas meritajs.");
    char tmp[10];
    // util::f2str(12.3+((float)n/2.1), tmp);
    float score = 90 + (float)random(0,100)/10;
    util::f2str(score, tmp);
    // sprintf(buffer, "BRUH %s Moment TOP %d %d %d", tmp, n*2, n*5, n*3);
    // sprintf(buffer, "Time: %ds V:%s\nScore: %s", n, "1",tmp);
    Serial.println(buffer);
    // util::printLcd(lcd1_conf, buffer);
    float temperature = dht_sensor.readTemperature();delay(500);
    float humidity = dht_sensor.readHumidity();delay(500);
    char tmp_temp[10];
    char tmp_hum[10];
    sprintf(buffer, "Temp: %s (*C)\nHum: %s (%%)", util::f2str(temperature, tmp_temp), util::f2str(humidity, tmp_hum));
    util::display_whole_text_LCD(lcd1_conf, buffer);
    // delay(1000);

    // Timer t = Timer(10*1000);
    // unsigned long last_event = 0;
    // while(!t.hasElapsed()){
    //     int noise_data = digitalRead(NOISE_PIN);
    //     if (noise_data == LOW){
    //       if (millis() - last_event > 25) {
    //         util::printLcd(lcd1_conf, "CLAP DETECTED");
    //         delay(100);
    //       }
    //       last_event = millis();
    //     }
    //     lcd1_conf.lcd.clear();
    // }
    // for (LinkedList<int>::Node* it = devices.getHead(); it != nullptr; it = it->next) {
    //   Serial.print(it->data);
    //   Serial.print(" ");
    // }
    // Serial.print("\n");

    // Benchmark stats.
    n++;
    long int total = util::benchmark(start);
    // print::printFreeMemory();
    print::total_time(total-1000);
}
