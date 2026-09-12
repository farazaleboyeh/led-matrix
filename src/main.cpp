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
  timerWrite(timer, 0);       // Reset counter back to zero
  timerAlarmEnable(timer);    // Arm for the new duration

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

  // timer = timerBegin(0, 80, true);
  // timerAttachInterrupt(timer, &on_timer, true);
  // timerAlarmWrite(timer, 50, false); // Just the initial kick-off duration
  // timerAlarmEnable(timer);

    // refresh_access_token();

  // TJpgDec.setJpgScale(8);
  // TJpgDec.setSwapBytes(false);
  // TJpgDec.setCallback(tjpg_output);

  display.clear();
  display.swap();
}

unsigned long last_frame_time = 0;
int current_frame = 0;

void loop()
{
// if (millis() - last_frame_time >= 1000) {
        
//         last_frame_time = millis(); 
        
//         display.clear();
//         if (current_frame == 0) {
//             display.set_pixel(0, 0, 255, 0, 0); 
//             current_frame = 1;
//         } else {
//             display.set_pixel(7, 3, 0, 0, 255); 
//             current_frame = 0;
//         }
//         display.swap();
//     }

  for(int i = 0; i < 10; i++){
    for(int j = 0; j < 10; j++){
      display.clear();
      display.set_pixel(j, i, 255, 255, 255); 

      display.swap();
      display.scan(200);
    }
  }
}

//   uint8_t *frame_data = display.bitplanes[current_row][current_bit];

//   Serial.print("Row ");
//   Serial.print(current_row);
//   Serial.print(" Bit ");
//   Serial.print(current_bit);
//   Serial.print(": ");

//   for (int i = 0; i < 5; i++)
//   {
//       // Print in binary format with leading zeros padded
//       for (int b = 7; b >= 0; b--)
//       {
//           Serial.print((frame_data[i] >> b) & 1);
//       }
//       Serial.print(" ");
//   }
//   Serial.println(); 
// }

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