// LCD DISPLEJS
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

namespace util
{
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
    int find_screen(){
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

namespace print
{
    void total_time(long int dif){
      Serial.print("Setup time: "); Serial.print(dif); Serial.println(" milliseconds");
    }
};

class Array
{
    // https://www.geeksforgeeks.org/overloading-subscript-or-array-index-operator-in-c/
private:
    int *ptr;
    int size;

public:
    // constructor for array class
    Array(int *p = NULL, int s = 0)
    {
        size = s;
        ptr = NULL;
        if (s != 0)
        {
            ptr = new int[s];
            for (int i = 0; i < s; i++)
                ptr[i] = p[i];
        }
    }

    // Overloading [] operator to access elements in array style
    // Implementation of [] operator.  This function must return a
    // reference as array element can be put on left side
    int &operator[](int index)
    {
        int idx = util::clamp(index, 0, this->size - 1);
        return this->ptr[idx];
    }

    // Utility function to print contents
    void print() const
    {
        for (int i = 0; i < this->size; i++)
        {
            Serial.print(this->ptr[i]);
            Serial.print(" ");
        }
        Serial.print("\n");
    }
    int sum()
    {
        int res = 0;
        for (int i = 0; i < this->size; i++)
            res += this->ptr[i];
        return res;
    }
};

class Screen
{
private:
    LiquidCrystal_I2C *LCD;
    int w;
    int h;
    char **grid;

public:
    Screen(LiquidCrystal_I2C *screen, int w = 1, int h = 1)
    {
        this->w = util::clamp(w, 0, 100);
        this->h = util::clamp(h, 0, 100);
        this->LCD = screen;
        this->grid = __generate_grid(w, h);
    }
    char **__generate_grid(int w, int h)
    {
        char **res = new char *[h];
        for (int i = 0; i < this->h; i++)
        {
            res[i] = util::char_arr(w);
        }
        return res;
    }

    void put(int i, int j, const char val)
    {
      // i: column idx
      // j: row idx
        i = util::clamp(i, 0, (this->h));
        j = util::clamp(j, 0, (this->w));
        this->grid[i][j] = val;
    }

    int put(int i, int j, char *arr)
    {
      int text_len = strlen(arr);
      int max_count = this->w - j;
      if (text_len > max_count){
        text_len = max_count;
      }
      int idx = 0;
      for (int col = j; col<j+text_len; col++){
        this->grid[i][col] = arr[idx];
        idx++;
      }
    }
    int put_num(int i, int j, float num){
      this->LCD->setCursor(j, i);
      this->LCD->print(num);
    }

    void fill(char symbol)
    {
        // Set every screen char position to a default symbol.
        for (int i = 0; i < this->h; i++)
        {
            for (int j = 0; j < this->w; j++)
            {
                this->put(i, j, symbol);
            }
        }
    }
    void write_buf_to_screen(){
        for (int i = 0; i < this->h; i++)
        {
            for (int j = 0; j < this->w; j++)
            {
                this->LCD->setCursor(j, i);
                this->LCD->print(this->grid[i][j]);
            }
        }
    }

    ~Screen()
    {
        for (int i = 0; i < this->h; i++)
        {
            delete[] this->grid[i];
        }
        delete [] this->grid;
    }


    void print()
    {
        for (int i = 0; i < this->h; i++)
        {
            Serial.print("LINE ");
            Serial.print(i);
            Serial.print(": ");
            for (int j = 0; j < this->w; j++)
            {
                Serial.print(this->grid[i][j]);
            }
            Serial.print("\n");
        }
        Serial.print("\n");
    }
};

class ScreenManager
{
    ScreenManager()
    {
    }
};

// Global variables
// Screens:
LiquidCrystal_I2C lcd1(0x27, 16, 2);

//
void setup()
{
    long int start = millis();
    util::init_serial();
    // Find screens
    int count = 0;
    while ((count = util::find_screen()) < 1){
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

void arr_test()
{
    int a[] = {100, 2, 4, 5, 6, 7, 8};
    Array arr(a, 7);
    arr[2] = 6;
    arr.print();
    arr[8] = 6;
    arr.print();
    int res = arr.sum();
    Serial.print(res);
    Serial.print("\n");
}
int n = 0;
void loop()
{
    // put your main code here, to run repeatedly:
    // Screen *s = new Screen(0x40, 16, 2);
    // delete s;
    long int start = millis();    
    Serial.print(n);
    Serial.print("\n");
    // lcd1.setCursor(0, 0);
    // lcd1.print("ABC");
    Screen s(&lcd1, 16, 2);
    // s.fill('-');
    // s.put(1, 5, 'T');
    // s.put(1, 6, 'E');
    // s.put(1, 7, 'S');
    // s.put(1, 8, 'T');
    // s.put(1, 26, "HELLO");
    // s.put(1, 0, "HELLO");

    // s.put(0, 0, "HI, the weather today was cloudy, but is sunny now.");
    // s.put_num(0, 0, 1337.883);
    // s.print();
    // s.write_buf_to_screen();
    lcd1.setCursor(0, 0);
    lcd1.print(123123.86);
    delay(1000);
    n++;

    // Benchmark stats.
    long int total = util::benchmark(start);
    print::total_time(total-1000);
}
