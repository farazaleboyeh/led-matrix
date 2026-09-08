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

  int next_delay = (1 << current_bit) * 30;
  timerAlarmWrite(timer, next_delay, true);

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

  // wifiMulti.addAP(WIFI_SSID1, WIFI_PASSWORD1);

  // while (wifiMulti.run() != WL_CONNECTED)
  // {
  //   delay(100);
  // }

  display.begin();

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &on_timer, true);
  timerAlarmWrite(timer, 10, true);
  timerAlarmEnable(timer);

  // refresh_access_token();

  // TJpgDec.setJpgScale(8);
  // TJpgDec.setSwapBytes(false);
  // TJpgDec.setCallback(tjpg_output);
  
  display.clear();
  // display.swap();

  // for(int i = 0; i < 10; i++){
  //   for(int j = 0; j < 10; j++){
  //     display.set_pixel(i, j, 255, 255, 255);
  //   }
  // }
  display.swap();
}

unsigned long last_frame_time = 0;
int current_frame = 0;

void loop()
{
  uint8_t t1[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
    display.shift_and_latch(t1, 5);
    Serial.println("Test 1: All 0x00");
    delay(2000);

    // 2. All 0xFF (3.3V on all pins)
    uint8_t t2[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    display.shift_and_latch(t2, 5);
    Serial.println("Test 2: All 0xFF");
    delay(2000);

    // 3. Columns 0xFF, Rows 0x00
    // (SR1-SR3 = 0xFF, SR4 cols=0xFF & rows=0x00 -> 0xFC, SR5 rows=0x00)
    uint8_t t3[5] = {0xFF, 0xFF, 0xFF, 0xFC, 0x00};
    display.shift_and_latch(t3, 5);
    Serial.println("Test 3: Cols 1, Rows 0");
    delay(2000);

    // 4. Columns 0x00, Rows 0xFF
    // (SR1-SR3 = 0x00, SR4 cols=0x00 & rows=0x01 -> 0x03, SR5 rows=0xFF)
    uint8_t t4[5] = {0x00, 0x00, 0x00, 0x03, 0xFF};
    display.shift_and_latch(t4, 5);
    Serial.println("Test 4: Cols 0, Rows 1");
    delay(2000);
  // if (millis() - last_frame_time >= 1000)
  // {
  //   last_frame_time = millis();
  //   if (current_frame == 0)
  //   {
  //     Serial.println("Frame 0");
  //     display.set_pixel(2, 2, 255, 255, 255);
  //     current_frame = 1;
  //   }
  //   else
  //   {
  //     Serial.println("Frame 1");
  //     display.set_pixel(0, 1, 255, 255, 255);
  //     current_frame = 0;
  //   }
  //   display.swap();
  // }
  // display.clear();
}

// unsigned long last_spotify_check = 0;
// const unsigned long SPOTIFY_POLL_INTERVAL = 2000;

// void loop()
// {
//   if (millis() - last_spotify_check >= SPOTIFY_POLL_INTERVAL)
//   {
//     last_spotify_check = millis();

//     if (WiFi.status() == WL_CONNECTED)
//     {
//       retrieve_artwork_url();
//     }
//     else
//     {
//       wifiMulti.run();
//     }
//   }
// }