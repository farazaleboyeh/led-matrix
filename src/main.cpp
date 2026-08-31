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
  if (current_bit >= 8){
    current_bit = 0;
    current_row++;
    if (current_row >= display.get_HEIGHT()){
      current_row = 0;
    }
  }
}

void setup()
{
  Serial.begin(9600);

  pinMode(latchp, OUTPUT);
  pinMode(clockp, OUTPUT);
  pinMode(datap, OUTPUT);

  rgb_init();
  rgb_set_color("white"); 
  
  wifiMulti.addAP(WIFI_SSID1, WIFI_PASSWORD1);

  while (wifiMulti.run() != WL_CONNECTED){
    delay(100);
  }

  if(WiFi.status() == WL_CONNECTED){
      rgb_set_color("green");
    }else{
      rgb_set_color("red");
    }
  
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &on_timer, true);
  timerAlarmWrite(timer, 10, true);
  timerAlarmEnable(timer);
}

void loop() {
    if (millis() - last_frame_time >= 1000) {
        
        last_frame_time = millis(); 
        
        display.clear();
        if (current_frame == 0) {
            display.set_pixel(0, 0, 255, 0, 0); // Red frame
            current_frame = 1;
        } else {
            display.set_pixel(2, 2, 0, 0, 255); // Blue frame
            current_frame = 0;
        }
        display.swap();
    }
}