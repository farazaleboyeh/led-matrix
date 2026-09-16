#pragma once

#include <bitset>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

struct SubPixel
{
    uint8_t byte_idx;
    uint8_t bit;
};

struct PinLocation
{
    SubPixel r;
    SubPixel g;
    SubPixel b;
};

struct RowLocation
{
    uint8_t byte_idx;
    uint8_t bit;
};

static const PinLocation COL_MAP[10] = {
    {{0, 7}, {0, 6}, {0, 5}},
    {{0, 4}, {0, 3}, {0, 2}},
    {{0, 1}, {0, 0}, {1, 7}},
    {{1, 6}, {1, 5}, {1, 4}},
    {{1, 3}, {1, 2}, {1, 1}},
    {{1, 0}, {2, 7}, {2, 6}},
    {{2, 5}, {2, 4}, {2, 3}},
    {{2, 2}, {2, 1}, {2, 0}},
    {{3, 7}, {3, 6}, {3, 5}},
    {{3, 4}, {3, 3}, {3, 2}}};

static const RowLocation ROW_MAP[10] = {
    {3, 1}, {3, 0}, {4, 7}, {4, 6}, {4, 5}, {4, 4}, {4, 3}, {4, 2}, {4, 1}, {4, 0}};

struct RGB
{
    uint8_t r, g, b;
};

class MatrixDriver
{
private:
    static const int WIDTH = 10;
    static const int HEIGHT = 10;
    static const int NUM_PIXELS = WIDTH * HEIGHT;
    static const int SHIFT_REGISTERS = ((WIDTH * 3) + HEIGHT) / 8;

    RGB framebuffer_a[NUM_PIXELS];
    RGB framebuffer_b[NUM_PIXELS];

    RGB *draw_buffer;
    RGB *show_buffer;

public:
    uint8_t bitplanes[HEIGHT][8][SHIFT_REGISTERS];
    RGB *target_art_buffer;

    MatrixDriver();
    void begin();
    void clear();
    void set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    void scan(uint32_t duration_ms);
    void swap();
    void unpack_bitplanes();
    void shift_and_latch(const uint8_t *data, size_t len);

    const RGB* get_show_buffer() const;

    int get_HEIGHT(); //lowkirkenuinely i think these are useless given they're read only, so figure out later
    int get_WIDTH();
};

void set_led(int x, int y, bool state);
void simple_led_cycle(void);
void set_row(int y, uint8_t row_pattern);