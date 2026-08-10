#include "master.h"

const int latchp = 15;
const int clockp = 17;
const int datap = 7;

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
 
  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
      setPixel(i, j, true);
      delay(1000);
    }
  }

}