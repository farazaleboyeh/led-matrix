#include "master.h"

MatrixDriver::MatrixDriver()
{
    clear();
}

void MatrixDriver::clear()
{
    memset(framebuffer, 0, sizeof(framebuffer));
}

void MatrixDriver::setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT){
        return;
    }
    int index = (y * WIDTH) + x;
    framebuffer[index] = {r, g, b};
}

void MatrixDriver::show()
{
    for (int y = 0; y < HEIGHT; y++)
    {
        uint16_t row_data = 0b1000000000000000 >> (9 + y);
        uint16_t col_data = 0;

        for (int x = 0; x < WIDTH; x++){
            int index = (y * WIDTH) + x;
            if (framebuffer[index].r > 0){
                col_data |= (0b1000000000000000 >> (x * 3));
            }
            if (framebuffer[index].g > 0){
                col_data |= (0b0100000000000000 >> (x * 3));
            }
            if (framebuffer[index].b > 0){
                col_data |= (0b0010000000000000 >> (x * 3));
            }
        }

        uint16_t output = ~(row_data | col_data);

        // std::string bitString = std::bitset<16>(output).to_string();
        // Serial.println(bitString.c_str());

        shift_and_latch(output);
        delayMicroseconds(1*1000*1000);
    }
}

void shift_and_latch(uint16_t thisLED)
{
    byte highByte = (thisLED >> 8) & 0xFF; // Top 8 bits
    byte lowByte = thisLED & 0xFF;         // Bottom 8 bits
    digitalWrite(latchp, LOW);             // Prevents output changes while shifting data
    shiftOut(datap, clockp, LSBFIRST, lowByte);
    shiftOut(datap, clockp, LSBFIRST, highByte);

    digitalWrite(latchp, HIGH); // Copies shifted data to output pins Q0–Q7
}

void set_led(int x, int y, bool state)
{
    if (x < 0 || x > 2 || y < 0 || y > 2)
    {
        return;
    }

    if (!state)
    {
        shift_and_latch(0xFFFF);
        return;
    }

    uint16_t row_data = 0b1000000000000000 >> (9 + y);
    uint16_t col_data = 0b1110000000000000 >> x * 3;

    uint16_t output = ~(row_data | col_data);
    shift_and_latch(output);
}

void simple_led_cycle()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            set_led(i, j, true);
            delay(500);
        }
    }
}

void set_row(int y, uint8_t row_pattern)
{
    if (y < 0 || y > 2)
    {
        return;
    }

    uint8_t row_data = 1 << (y + 4);
    uint16_t col_data = bit_tripler[row_pattern & 0b111];

    uint16_t output = ~(row_data | col_data);
    shift_and_latch(output);
}