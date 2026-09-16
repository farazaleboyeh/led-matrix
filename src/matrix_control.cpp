#include "master.h"

MatrixDriver::MatrixDriver()
{
    draw_buffer = framebuffer_a;
    show_buffer = framebuffer_b;
    clear();
}

void MatrixDriver::begin()
{
    digitalWrite(latchp, LOW);

    SPI.begin(clockp, -1, datap, -1);

    memset(draw_buffer, 0, NUM_PIXELS * sizeof(RGB));
    memset(show_buffer, 0, NUM_PIXELS * sizeof(RGB));
    memset(bitplanes, 0, sizeof(bitplanes));

    static const uint8_t blank[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    shift_and_latch(blank, 5);
}

void MatrixDriver::clear()
{
    memset(draw_buffer, 0, NUM_PIXELS * sizeof(RGB));
}

void MatrixDriver::set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
    {
        return;
    }
    int index = (y * WIDTH) + x;
    draw_buffer[index] = {r, g, b};
}

void MatrixDriver::scan(uint32_t duration_ms)
{
    static const uint8_t blank[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    uint32_t start = millis();
    while (millis() - start < duration_ms)
    {
        for (int y = 0; y < 10; y++)
        {
            for (int bit = 0; bit < 8; bit++)
            {
                shift_and_latch(bitplanes[y][bit], 5);
                int delay_time = (1 << bit) * 5; 
                delayMicroseconds(delay_time);
              
            }
            shift_and_latch(blank, 5);
        }
    }
}

void MatrixDriver::unpack_bitplanes()
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int bit = 0; bit < 8; bit++)
        {
            memset(bitplanes[y][bit], 0xFF, 5); // clear all 5 bytes for this row & BCM slice (facilitates bitwise or later)

            for (int x = 0; x < WIDTH; x++)
            {
                int index = (y * WIDTH) + x;
                RGB p = show_buffer[index];
                const PinLocation &loc = COL_MAP[x];

                if (p.r & (1 << bit))
                {
                    bitplanes[y][bit][loc.r.byte_idx] &= ~(1 << loc.r.bit);
                }
                if (p.g & (1 << bit))
                {
                    bitplanes[y][bit][loc.g.byte_idx] &= ~(1 << loc.g.bit);
                }
                if (p.b & (1 << bit))
                {
                    bitplanes[y][bit][loc.b.byte_idx] &= ~(1 << loc.b.bit);
                }
            }
            const RowLocation &rloc = ROW_MAP[y];
            bitplanes[y][bit][rloc.byte_idx] &= ~(1 << rloc.bit);
        }
    }
}

void MatrixDriver::swap()
{
    RGB *temp = show_buffer;
    show_buffer = draw_buffer;
    draw_buffer = temp;

    unpack_bitplanes();
}

void MatrixDriver::shift_and_latch(const uint8_t *data, size_t len)
{
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
    for (int i = len - 1; i >= 0; i--)
    {
        SPI.transfer(data[i]);
    }

    SPI.endTransaction();

    digitalWrite(latchp, HIGH);
    digitalWrite(latchp, LOW);
}

const RGB* MatrixDriver::get_show_buffer() const 
{ 
    return show_buffer; 
}

int MatrixDriver::get_HEIGHT()
{
    return HEIGHT;
}

int MatrixDriver::get_WIDTH()
{
    return WIDTH;
}

// uint16_t MatrixDriver::get_bitplanes(){
//     return bitplanes;
// }

// void set_led(int x, int y, bool state){
//     if (x < 0 || x > 2 || y < 0 || y > 2)
//     {
//         return;
//     }

//     if (!state)
//     {
//         shift_and_latch(0xFFFF);
//         return;
//     }

//     uint16_t row_data = 0b1000000000000000 >> (9 + y);
//     uint16_t col_data = 0b1110000000000000 >> x * 3;

//     uint16_t output = ~(row_data | col_data);
//     shift_and_latch(output);
// }

// void simple_led_cycle()
// {
//     for (int i = 0; i < 3; i++)
//     {
//         for (int j = 0; j < 3; j++)
//         {
//             set_led(i, j, true);
//             delay(500);
//         }
//     }
// }

// void set_row(int y, uint8_t row_pattern)
// {
//     if (y < 0 || y > 2)
//     {
//         return;
//     }

//     uint8_t row_data = 1 << (y + 4);
//     uint16_t col_data = bit_tripler[row_pattern & 0b111];

//     uint16_t output = ~(row_data | col_data);
//     shift_and_latch(output);
// }