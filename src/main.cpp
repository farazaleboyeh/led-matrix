#include "master.h" 
#include <WiFiMulti.h>

const int latchp = 15;
const int clockp = 17;
const int datap = 7;

WiFiMulti wifiMulti; 

unsigned long lastRefresh = 0;
const unsigned long refreshInterval = 55 * 60 * 1000; 
float duration, distance;

uint16_t LED = 0x0000;

void shiftLED(uint16_t thisLED)
{
  Serial.println(thisLED);
  byte highByte = (thisLED >> 8) & 0xFF; // Get the top 8 bits
  byte lowByte = thisLED & 0xFF;        // Get the bottom 8 bits
  digitalWrite(latchp, LOW); //Prevents output changes while shifting data
  shiftOut(datap, clockp, LSBFIRST, lowByte);
  shiftOut(datap, clockp, LSBFIRST, highByte);

  digitalWrite(latchp, HIGH); //Copies shifted data to output pins Q0–Q7
}

void setLED(){
  //top left
  // LED = 0b00111111100111111;
  shiftLED(LED); 

  delay(10);

  // //bottom right
  // LED = 0b1110001011111111;
  // shiftLED(LED); 

   delay(1000);
}

void setup()
{
  Serial.begin(115200);

  rgb_init();
  rgb_set_color("white");

  pinMode(latchp, OUTPUT);
  pinMode(clockp, OUTPUT);
  pinMode(datap, OUTPUT);

  wifiMulti.addAP("KiaFar Home", "Loobia12!!");
  wifiMulti.addAP("BELL326", "D96F1CC171AA");

  // while (wifiMulti.run() != WL_CONNECTED){
  //   delay(100);
  // }
  // Serial.println("Connected");

  refresh_access_token();
  //skip_song();
  
  
}

void loop()
{
  setLED();
  // for(int i=0;i<16;i++){
  //   shiftLED(1 << i);
  //   delay(200);
  // }

  // unsigned long currentMillis = millis();
  // if (WiFi.status() == WL_CONNECTED){
  //   rgb_set_color("green");

  //   if (currentMillis - lastRefresh >= refreshInterval) {
  //     refresh_access_token();
  //     lastRefresh = currentMillis; 
  //   }    
  // }
  // else{
  //   rgb_set_color("red");
  // }

}