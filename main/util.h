#ifndef UTIL_H
#define UTIL_H

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "Measurement.h"

namespace util {
    void init_serial(unsigned int baud_rate = 9600);
    int clamp(int n, int lower, int upper);
    unsigned long benchmark(unsigned long t1);
    long int benchmark(long int t1, long int t2);
    char *f2str(float value, char *result);
    bool float_compare(float x, float y, float tolerance);
    void write_measurement_to_file(File file, measurement::entry data);
    void read_struct_from_file(File file, measurement::entry &data);
    measurement::entry read_avg_struct_from_file(File file, measurement::entry &data);
};

#endif // UTIL_H
