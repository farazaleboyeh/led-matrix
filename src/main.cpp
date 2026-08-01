#include "master.h"


const int latchp = 15;
const int clockp = 17;
const int datap = 7;


uint16_t LED = 0xFFFF;

void shiftLED(uint16_t thisLED)
{
  Serial.println(thisLED);
  byte highByte = (thisLED >> 8) & 0xFF; // Get the top 8 bits
  byte lowByte = thisLED & 0xFF;         // Get the bottom 8 bits
  digitalWrite(latchp, LOW);             // Prevents output changes while shifting data
  shiftOut(datap, clockp, LSBFIRST, lowByte);
  shiftOut(datap, clockp, LSBFIRST, highByte);

  digitalWrite(latchp, HIGH); // Copies shifted data to output pins Q0–Q7
}

void setLED()
{

}

void setup()
{
  Serial.begin(115200);

  rgb_init();
  rgb_set_color("white");

 

}

void loop()
{

}