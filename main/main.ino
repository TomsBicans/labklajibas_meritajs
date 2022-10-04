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
    LiquidCrystal_I2C *scrn;
    int w;
    int h;
    char **grid;

public:
    Screen(LiquidCrystal_I2C *scrn = NULL, int w = 1, int h = 1)
    {
        this->w = util::clamp(w, 0, 100);
        this->h = util::clamp(h, 0, 100);
        this->scrn = scrn;
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

    void debug_screen_val(int i, int j, const char *loc){
      Serial.print("idx loc:");
      Serial.print(i);
      Serial.print(' ');
      Serial.print(j);
      Serial.print("  Value:");
      Serial.print(*loc);
      Serial.print("Address:");
      Serial.print(*loc, HEX);
    }

    void put(int i, int j, const char val)
    {
      // i: column idx
      // j: row idx
        i = util::clamp(i, 0, (this->h));
        j = util::clamp(j, 0, (this->w));
        this->grid[i][j] = val;
    }

    void put(int i, int j, char *arr)
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
            // Serial.print(this->grid[i]);
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

void setup()
{
    util::init_serial();
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
    Serial.print(n);
    Serial.print("\n");
    Screen s(NULL, 30, 4);
    s.fill('-');
    s.put(1, 5, 'T');
    s.put(1, 6, 'E');
    s.put(1, 7, 'S');
    s.put(1, 8, 'T');
    s.put(2, 0, "HELLO");
    s.print();
    delay(2000);
    n++;
}
