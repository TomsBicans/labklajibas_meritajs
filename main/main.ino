// LCD DISPLEJS
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

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
    int find_I2C_devices(){
        byte error, address;
        int nDevices;
        int res = 0;        

        Serial.println("Scanning...");

        nDevices = 0;
        for (address = 1; address < 127; address++ ) {
          Wire.begin();
          Wire.beginTransmission(address);
          error = Wire.endTransmission();
          if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16)
              Serial.print("0");
            Serial.print(address, HEX);
            Serial.println("  !");
            res++;
            nDevices++;
          }
          else if (error == 4) {
            Serial.print("Unknown error at address 0x");
            if (address < 16)
              Serial.print("0");
            Serial.println(address, HEX);
          }
        }
        if (res == 0){
          Serial.println("No I2C devices found\n");
          return 0;
        }
        else{
          Serial.println("done\n");
          return res;
        }}
};

namespace print{
    void total_time(long int dif){
      Serial.print("Setup time: "); Serial.print(dif); Serial.println(" milliseconds");
    }
};

// GLOBAL VARIABLES
// SCREENS:
LiquidCrystal_I2C lcd1(0x27, 16, 2);

//
void setup()
{
    long int start = millis();
    util::init_serial();
    // Find screens
    int count = 0;
    while ((count = util::find_I2C_devices()) < 1){
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
    Serial.print(n);
    Serial.print("\n");
    // lcd1.setCursor(0, 0);
    // lcd1.print("ABC");  
    lcd1.setCursor(0, 0);
    lcd1.print(123123.86);
    delay(1000);
    n++;

    // Benchmark stats.
    long int total = util::benchmark(start);
    print::total_time(total-1000);
}
