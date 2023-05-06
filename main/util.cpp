#include "util.h"

namespace util{
    void init_serial(unsigned int baud_rate)
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