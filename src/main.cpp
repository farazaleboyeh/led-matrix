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
  // display.setPixel(0, 0, 0b11111111, 0b11111111, 0b11111111);
  // display.show();
  
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      display.setPixel(j, i, 0b11111111, 0b11111111, 0);
      display.show();
      // delay(500);
      display.setPixel(j, i, 0, 0b11111111, 0b11111111);
      display.show();
      // delay(500);
      display.setPixel(j, i, 0b11111111, 0, 0b11111111);
      display.show();
      // delay(500);
    
      // delay(500);
      display.clear();
    }
  }

  
  // set_row(0, 0b010);
  // delay(500);
  // set_row(1, 0b101);
  // delay(500);
  // set_row(2, 0b111);
  // delay(500);
}