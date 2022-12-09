// LCD DISPLEJS
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "LinkedList.h"
#include "LCD_config.h"
#include <stdio.h>
// #include <MemoryFree.h>


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
    void printLcd(lcd_config config, char text[]) {
      int numCols = config.columns; // Assume a 16-column LCD screen
      int numRows = config.rows; // Assume a 2-row LCD screen
      int textLen = strlen(text); // Get the length of the text array
      int col = 0, row = 0; // Start at the first column and first row

      for (int i = 0; i < textLen; i++) {
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
    }
    void f2str(float value, char *result) {
      // Convert the floating-point number to a string with two decimal places
      // and a minimum width of 8 characters (including the decimal point and the sign)
      dtostrf(value, 2, 1, result);
    }
};


namespace print{
    void total_time(long int dif){
      Serial.print("Total loop time: "); Serial.print(dif); Serial.println(" milliseconds");
    }
  //   void printFreeMemory() {
  //   // Print the amount of free memory to the serial monitor
  //   Serial.print("Free memory: ");
  //   Serial.print(freeMemory());
  //   Serial.println(" bytes");
  // }
};

// GLOBAL VARIABLES
// SCREENS:
LiquidCrystal_I2C lcd1(0x27, 16, 2);

lcd_config lcd1_conf = {lcd1, 16, 2};
//
void setup()
{
    long int start = millis();
    util::init_serial();
    // Find screens
    int count = 0;
    while ((count = util::find_I2C_devices(false).getSize()) < 1){
      Serial.print("Screens found: "); Serial.println(count);
      delay(2000);
    }
    Serial.print("SCREEN COUNT: "); Serial.println(count);
    // Initialize screens.
    lcd1.init();
    lcd1.backlight();
    // Initialize sensors.
    // Initialize result storage.

    // Benchmark stats.
    long int total = util::benchmark(start);
    print::total_time(total);
}

int n = 0;
void loop()
{
    // put your main code here, to run repeatedly:
    long int start = millis();
    LinkedList<int> devices = util::find_I2C_devices();
    // lcd1.print(devices.getFirst(), HEX);
    
    char tmp[10];
    // util::f2str(12.3+((float)n/2.1), tmp);
    util::f2str(99.7, tmp);
    char buffer[100];
    // sprintf(buffer, "BRUH %s Moment TOP %d %d %d", tmp, n*2, n*5, n*3);
    sprintf(buffer, "Time: %ds V:%s\nVertejums: %s", n, "1",tmp);
    Serial.println(buffer);
    util::printLcd(lcd1_conf, buffer);
    delay(1000);
    
    for (LinkedList<int>::Node* it = devices.getHead(); it != nullptr; it = it->next) {
      Serial.print(it->data);
      Serial.print(" ");
    }
    Serial.print("\n");

    // Benchmark stats.
    n++;
    long int total = util::benchmark(start);
    // print::printFreeMemory();
    print::total_time(total-1000);
}
