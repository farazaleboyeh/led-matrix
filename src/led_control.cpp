#include "master.h"

const int RGB_BRIGHTNESS_CUSTOM = 10;
const int RGB_PIN = 48; 

Adafruit_NeoPixel pixel(1, RGB_PIN, NEO_GRB + NEO_KHZ800);

void rgb_init(){
    pixel.begin();
    pixel.setBrightness(RGB_BRIGHTNESS_CUSTOM);
}

void rgb_set_color(String color){
    if(color == "white"){
        pixel.setPixelColor(0, pixel.Color(255, 255, 255)); 
    }
    else if(color == "green"){
        pixel.setPixelColor(0, pixel.Color(0, 255, 0));
    }
    else if(color == "red"){
        pixel.setPixelColor(0, pixel.Color(255, 0, 0));
    }
    else{
        pixel.setPixelColor(0, pixel.Color(0, 0, 255)); //blue fallback
    }
    pixel.show();    
}