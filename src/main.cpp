#include <Arduino.h>
#include <master.h>

const int DATA_PIN  = datap;   
const int CLOCK_PIN = clockp;  
const int LATCH_PIN = latchp;  

//receives 5 bytes (1 per shift register)
void send_raw(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5)
{
    digitalWrite(LATCH_PIN, LOW);

    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, b5);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, b4);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, b3);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, b2);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, b1);
    
    digitalWrite(LATCH_PIN, HIGH);
    digitalWrite(LATCH_PIN, LOW);
}

// 0b 7 6 5 4 3 2 1 0

void setup()
{
    Serial.begin(115200);
    delay(500);

    pinMode(DATA_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);

    digitalWrite(LATCH_PIN, LOW);
    digitalWrite(CLOCK_PIN, LOW);
    digitalWrite(DATA_PIN, LOW);

    delay(2000);
}

void loop()
{
  send_raw(0b11111111, 0b11110101, 0b11111111, 0b11111111, 0b00111111);
  delay(500);
}