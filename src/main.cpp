#include <Arduino.h>
#include <master.h>

const int DATA_PIN  = datap;   
const int CLOCK_PIN = clockp;  
const int LATCH_PIN = latchp;  

//receives 5 bytes (1 per shift register)
void send_raw(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5)
{
    digitalWrite(LATCH_PIN, LOW);
  
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, b1);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, b2);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, b3);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, b4);
    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, b5);
    
    digitalWrite(LATCH_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(LATCH_PIN, LOW);
}

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

    Serial.println("--- Bit-Bang Sanity Test Starting ---");
}

void loop()
{
    Serial.println("Pushing ALL 0x00...");
    send_raw(0x00, 0x00, 0x00, 0x00, 0x00);
    delay(2000);

    // Serial.println("Pushing ALL 0xFF...");
    // send_raw(0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
    // delay(2000);
}