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
}

void loop()
{
  display.clear();
  

  display.set_pixel(0, 0, 255, 0, 0);
  display.set_pixel(1, 1, 20, 10, 160);


  display.swap();

  for (int i = 0; i < 50; i++)
  {
    display.scan();
  }
}