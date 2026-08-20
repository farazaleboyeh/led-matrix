#pragma once

#include <bitset>
#include <cstdint>
#include <cstring>
#include <iostream>

const uint16_t bit_tripler[8] = {
  0b0000000000000000, 
  0b0000001110000000, 
  0b0001110000000000, 
  0b0001111110000000, 
  0b1110000000000000, 
  0b1110001110000000,
  0b1111110000000000,
  0b1111111110000000 
};

struct RGB {
    uint8_t r, g, b;
};

class MatrixDriver {
private: 
    static const int WIDTH = 3;
    static const int HEIGHT = 3;
    static const int NUM_PIXELS = WIDTH * HEIGHT;
    RGB framebuffer[NUM_PIXELS];
public:
    MatrixDriver();
    void clear();  
    void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    void show();   
};

void shift_and_latch(uint16_t thisLED);
void set_led(int x, int y, bool state);
void simple_led_cycle(void);
void set_row(int y, uint8_t row_pattern);