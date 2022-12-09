#include <LiquidCrystal_I2C.h>
struct lcd_config {
  LiquidCrystal_I2C& lcd;
  int columns;
  int rows;
};