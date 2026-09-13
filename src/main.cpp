#include "master.h"

MatrixDriver display;
hw_timer_t *timer = NULL;
WiFiMulti wifiMulti;

volatile int current_row = 0;
volatile int current_bit = 0;

void IRAM_ATTR on_timer()
{
  uint8_t *frame_data = display.bitplanes[current_row][current_bit];
  display.shift_and_latch(frame_data, 5);

  int next_delay = (1 << current_bit) * 5;
  timerAlarmWrite(timer, next_delay, false);
  timerWrite(timer, 0);    
  timerAlarmEnable(timer); 

  current_bit++;
  if (current_bit >= 8)
  {
    current_bit = 0;
    current_row++;
    if (current_row >= 10)
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
      int px = x + i + 1;
      int py = y + j + 1;

      if (px < 10 && py < 10 && px >= 0 && py >= 0)
      {
        uint16_t color = bitmap[j * w + i];

        uint8_t r = ((color >> 11) & 0x1F) << 3;
        uint8_t g = ((color >> 5)  & 0x3F) << 2;
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

  wifiMulti.addAP(WIFI_SSID1, WIFI_PASSWORD1);

  while (wifiMulti.run() != WL_CONNECTED)
  {
    delay(100);
  }

  display.begin();

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &on_timer, true);
  timerAlarmWrite(timer, 50, false); 
  timerAlarmEnable(timer);

  refresh_access_token();

  TJpgDec.setJpgScale(8);
  TJpgDec.setSwapBytes(false);
  TJpgDec.setCallback(tjpg_output);

  display.clear();
  display.swap();
}

unsigned long last_spotify_check = 0;
const unsigned long SPOTIFY_POLL_INTERVAL = 2000;

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
      wifiMulti.run();
    }
  }
}