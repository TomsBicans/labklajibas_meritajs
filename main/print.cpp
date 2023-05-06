#include "print.h"

namespace print {
    void total_time(long int dif) {
        Serial.print("Total loop time: ");
        Serial.print(dif);
        Serial.println(" milliseconds");
    }
} // namespace print
