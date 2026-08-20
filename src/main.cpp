#include "master.h"

MatrixDriver display;

void setup()
{
  Serial.begin(9600);

  pinMode(latchp, OUTPUT);
  pinMode(clockp, OUTPUT);
  pinMode(datap, OUTPUT);

  rgb_init();
  rgb_set_color("red");

  display.setPixel(0, 0, 0b11111111, 0b11111111, 0b11111111);
  display.show();
  
}

void loop()
{
  // set_row(0, 0b010);
  // delay(500);
  // set_row(1, 0b101);
  // delay(500);
  // set_row(2, 0b111);
  // delay(500);
}