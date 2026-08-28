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
  display.set_pixel(0, 0, 0b11111111, 0b11111111, 0b1111111);
  
  display.swap();

  for(int i = 0; i < 50; i++) {
        display.scan_matrix(); 
  }
}