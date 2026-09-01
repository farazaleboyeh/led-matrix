#include "master.h"

MatrixDriver display;
hw_timer_t *timer = NULL;
WiFiMulti wifiMulti;

volatile int current_row = 0;
volatile int current_bit = 0;

unsigned long last_frame_time = 0;
int current_frame = 0;

void IRAM_ATTR on_timer()
{
  display.shift_and_latch(0xFFFF);

  uint16_t row_data = 0b1000000000000000 >> (9 + current_row);
  uint16_t col_data = display.bitplanes[current_row][current_bit];

  uint16_t output = ~(row_data | col_data);
  display.shift_and_latch(output);

  int next_delay = (1 << current_bit) * 5;
  timerAlarmWrite(timer, next_delay, true);

  current_bit++;
  if (current_bit >= 8)
  {
    current_bit = 0;
    current_row++;
    if (current_row >= display.get_HEIGHT())
    {
      current_row = 0;
    }
  }
}

bool tjpg_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap)
{
  for (int j = 0; j < h; j++)
  {
    for (int i = 0; i < w; i++)
    {
      int px = (x + i) / 4;
      int py = (y + j) / 4;

      if (px < 2 && py < 2)
      {
        uint16_t color = bitmap[j * w + i];

        uint8_t r = ((color >> 11) & 0x1F) << 3;
        uint8_t g = ((color >> 5) & 0x3F) << 2;
        uint8_t b = (color & 0x1F) << 3;

        display.set_pixel(px, py, r, g, b);
      }
    }
  }
  return 1;
}

void setup()
{
  Serial.begin(115200);

  pinMode(latchp, OUTPUT);
  pinMode(clockp, OUTPUT);
  pinMode(datap, OUTPUT);

  rgb_init();
  rgb_set_color("white");

  wifiMulti.addAP(WIFI_SSID1, WIFI_PASSWORD1);

  while (wifiMulti.run() != WL_CONNECTED)
  {
    delay(100);
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    rgb_set_color("green");
  }
  else
  {
    rgb_set_color("red");
  }

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &on_timer, true);
  timerAlarmWrite(timer, 10, true);
  timerAlarmEnable(timer);

  refresh_access_token();

  TJpgDec.setJpgScale(8);         
  TJpgDec.setSwapBytes(true);       
  TJpgDec.setCallback(tjpg_output);
}

// void loop() {
//   int x = retrieve_artwork_url();
//   Serial.println(x);
//     // if (millis() - last_frame_time >= 1000) {

//     //     last_frame_time = millis();

//     //     display.clear();
//     //     if (current_frame == 0) {
//     //         display.set_pixel(0, 0, 255, 0, 0); // Red frame
//     //         current_frame = 1;
//     //     } else {
//     //         display.set_pixel(2, 2, 0, 0, 255); // Blue frame
//     //         current_frame = 0;
//     //     }
//     //     display.swap();
//     // }
// }

unsigned long last_spotify_check = 0;
const unsigned long SPOTIFY_POLL_INTERVAL = 5000;

void loop()
{
  if (millis() - last_spotify_check >= SPOTIFY_POLL_INTERVAL)
  {
    last_spotify_check = millis();

    if (WiFi.status() == WL_CONNECTED)
    {
      retrieve_artwork_url();
    }
    else
    {
      wifiMulti.run(); // Attempt reconnect
    }
  }
}