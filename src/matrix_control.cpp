#include "master.h"

MatrixDriver::MatrixDriver()
{
    draw_buffer = framebuffer_a;
    show_buffer = framebuffer_b;
    clear();
}

void MatrixDriver::begin()
{
    // 1. Configure the Latch Pin
    // pinMode(latchp, OUTPUT);
    digitalWrite(latchp, LOW);

    // // Optional: If you connected 74HC595 OE pin to a GPIO, hold it HIGH (disabled)
    // // during boot to prevent bright flashes before data is loaded.
    // #ifdef oep
    // pinMode(oep, OUTPUT);
    // digitalWrite(oep, HIGH);
    // #endif

    // 2. Map SPI to YOUR hardware pins: SPI.begin(SCK, MISO, MOSI, SS)
    // -1 means the pin is unused by our shift registers
    SPI.begin(clockp, -1, datap, -1);

    // 3. Set bus speed to 10 MHz, MSB first, Mode 0
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));

    // 4. Clear all internal memory buffers
    memset(draw_buffer, 0, NUM_PIXELS * sizeof(RGB));
    memset(show_buffer, 0, NUM_PIXELS * sizeof(RGB));
    memset(bitplanes, 0, sizeof(bitplanes));

    // 5. Shift out a blank frame to turn off all LEDs cleanly at boot
    static const uint8_t blank[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    shift_and_latch(blank, 5);

    #ifdef oep
    digitalWrite(oep, LOW); // Enable outputs now that registers are zeroed
    #endif
}

void MatrixDriver::clear()
{
    memset(draw_buffer, 0xFF, NUM_PIXELS * sizeof(RGB));
}

void MatrixDriver::set_pixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
    {
        return;
    }
    int index = (y * WIDTH) + x;
    draw_buffer[index] = {r, g, b};

    Serial.println(index);
    Serial.println(std::bitset<8>(r).to_string().c_str());
    Serial.println(std::bitset<8>(g).to_string().c_str());
    Serial.println(std::bitset<8>(b).to_string().c_str());
    Serial.println("---");
}

void MatrixDriver::scan()
{
    static const uint8_t blank[5] = {0x00, 0x00, 0x00, 0x00, 0x00};

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

void MatrixDriver::unpack_bitplanes()
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int bit = 0; bit < 8; bit++)
        {
            memset(bitplanes[y][bit], 0, 5); // clear all 5 bytes for this row & BCM slice

            for (int x = 0; x < WIDTH; x++)
            {
                int index = (y * WIDTH) + x;
    
                RGB p = show_buffer[index];

                const PinLocation &loc = COL_MAP[x];

                if (p.r & (1 << bit))
                {
                    bitplanes[y][bit][loc.r.byte_idx] |= (1 << (7 - loc.r.bit));
                }

                if (p.g & (1 << bit))
                {
                    bitplanes[y][bit][loc.g.byte_idx] |= (1 << (7 - loc.g.bit));
                }

                if (p.b & (1 << bit))
                {
                    bitplanes[y][bit][loc.b.byte_idx] |= (1 << (7 - loc.b.bit));
                }


          
            }
            const RowLocation &rloc = ROW_MAP[y];
            bitplanes[y][bit][rloc.byte_idx] |= (1 << (7 - rloc.bit));
            bitplanes[y][bit][rloc.byte_idx] = ~(bitplanes[y][bit][rloc.byte_idx]);

            Serial.println(std::bitset<8>(bitplanes[y][bit][rloc.byte_idx]).to_string().c_str());
            
        }
        Serial.println("-----");
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
    SPI.writeBytes(data, len);
    digitalWrite(latchp, HIGH);
    digitalWrite(latchp, LOW);
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