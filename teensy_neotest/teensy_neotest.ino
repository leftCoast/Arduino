#include <mapper.h>
#include <lists.h>
#include <colorObj.h>
#include <neoPixel.h>
#include <timeObj.h>

#define NUM_LEDS          150
#define PIXEL_PIN         2
#define DARK_PERCENT      60


neoPixel lites(NUM_LEDS, PIXEL_PIN);

colorObj flame(WHITE);
colorObj backColor(BLUE);

void setup() {

  //Serial.begin(9600);

  //flame.blend(&yellow,20);
  flame.blend(&black,DARK_PERCENT);
  flame.blend(&black,DARK_PERCENT);
  backColor.blend(&black,DARK_PERCENT);
  backColor.blend(&black,DARK_PERCENT);
  
  lites.begin();
  lites.setAll(&backColor);
  lites.setPixelColor(0,&flame);
  flame.blend(&black,DARK_PERCENT);
  lites.setPixelColor(1,&flame);
  flame.blend(&black,DARK_PERCENT);
  lites.setPixelColor(2,&flame);
}


void loop() {

  lites.roll();
  delay(200);
  lites.show();
}
