#include <Arduino.h>
#include "operation.h"
#include "PinDefinitions.h"


operation::BUTTON operation::button_press(unsigned long time_limit, operation::BUTTON default_mode) {
  operation::BUTTON btn = default_mode;
  Timer t = Timer(time_limit);
  while (!t.hasElapsed()) {
    int tmp_mode = digitalRead(USER_INPUT_PIN_1);
    if (tmp_mode == HIGH) {
      btn = operation::BTN1;
      break;
    }
    tmp_mode = digitalRead(USER_INPUT_PIN_2);
    if (tmp_mode == HIGH) {
      btn = operation::BTN2;
      break;
    }
    delay(20);
  }
  return btn;
}
