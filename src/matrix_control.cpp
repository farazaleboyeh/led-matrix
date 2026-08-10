#include "master.h"

const int latchp = 15;
const int clockp = 17;
const int datap = 7;

void shiftLED(uint16_t thisLED)
{
    byte highByte = (thisLED >> 8) & 0xFF; // Top 8 bits
    byte lowByte = thisLED & 0xFF;         // Bottom 8 bits
    digitalWrite(latchp, LOW);             // Prevents output changes while shifting data
    shiftOut(datap, clockp, LSBFIRST, lowByte);
    shiftOut(datap, clockp, LSBFIRST, highByte);

    digitalWrite(latchp, HIGH); // Copies shifted data to output pins Q0–Q7
}

void setPixel(int x, int y, bool state)
{
    if(x < 0 || x > 2 || y < 0 || y > 2){
        return;
    }

    if(!state){
        shiftLED(0xFFFF);
        return;
    }

    byte rowData = 1 << (y + 4);
    uint16_t colData = 0b1110000000000000 >> x * 3;
    //Serial.println(std::bitset<8>(rowData).to_string().c_str());
    //Serial.println(std::bitset<16>(colData).to_string().c_str());

    uint16_t output = ~(colData | rowData);

    //Serial.println(std::bitset<16>(output).to_string().c_str());

    shiftLED(output);
}
