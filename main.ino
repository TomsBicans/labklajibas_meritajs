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
    unsigned char *char_arr(int size)
    {
        size = clamp(size, 0, abs(size));
        unsigned char *t = new unsigned char[size + 1];
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
    unsigned char **grid;

public:
    Screen(LiquidCrystal_I2C *scrn = NULL, int w = 1, int h = 1)
    {
        this->w = util::clamp(w, 0, 100);
        this->h = util::clamp(h, 0, 100);
        this->scrn = scrn;
        this->grid = __generate_grid(w, h);
    }
    unsigned char **__generate_grid(int w, int h)
    {
        unsigned char **res = new unsigned char *[h];
        for (int i = 0; i < this->h; i++)
        {
            res[i] = util::char_arr(w);
        }
        return res;
    }

    void put(int w_x, int h_x, unsigned char val)
    {
        w_x = util::clamp(w_x, 0, (this->w) - 1);
        h_x = util::clamp(h_x, 0, (this->h) - 1);
        this->grid[w_x][h_x] = val;
    }

    void init()
    {
        // Set every screen char position to _
        for (int i = 0; i < this->h; i++)
        {
            for (int j = 0; j < this->w; j++)
            {
                put(i, j, "_");
            }
        }
    }

    // int &operator[](int index)
    // {
    //     // int idx = util::clamp(index, 0, this->size - 1);
    //     return this->grid[index];
    // }

    // void print()
    // {
    //     for (int i = 0; i < this->h; i++)
    //     {
    //         Serial.print("LINE ");
    //         Serial.print(i);
    //         Serial.print(": ");
    //         for (int j = 0; j < this->w; j++)
    //         {
    //             Serial.print("X");
    //         }
    //         Serial.print("\n");
    //     }
    //     Serial.print("\n");
    // }
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

void loop()
{
    // put your main code here, to run repeatedly:
    Screen s(NULL, 16, 2);
    s.init();
    s.print();
    delay(1000);
}
