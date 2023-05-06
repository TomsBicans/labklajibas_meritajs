#ifndef OPERATION_H
#define OPERATION_H

#include "timer.h"

namespace operation {
    enum BUTTON {
      BTN1,
      BTN2
    };

    BUTTON button_press(unsigned long time_limit = 10 * 1000, BUTTON default_mode = BTN1);
}

#endif  // OPERATION_H
