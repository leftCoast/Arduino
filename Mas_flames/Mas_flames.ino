#include <mapper.h>
#include <lists.h>
#include <colorObj.h>
#include <neoPixel.h>
#include <timeObj.h>

#define NUM_LEDS          150
#define PIXEL_PIN         2
//#define DARK_PERCENT      75

neoPixel lites(NUM_LEDS, PIXEL_PIN);

colorObj flame(RED);

void setup() {

  //Serial.begin(9600);

  flame.blend(&yellow,20);
  
  lites.begin();
  lites.setAll(&black);
  lites.show();
}


void loop() {

  int       flameLen;
  int       taperLen;
  mapper    taperPercent;
  colorObj  localFlame;
  
  lites.setAll(&black);
  flameLen = random(25,40);
  taperLen = random(12,flameLen/2);
  taperPercent.setValues(flameLen,flameLen-taperLen,90,0);
  for(int i=0;i<flameLen;i++) {
    localFlame.setColor(&flame);
    localFlame.blend(&black,tapperPercent.Map(i));
    lites.setPixelColor(i,&localFlame);
  }

  taperLen = random(1,flameLen/2);
  tapperPercent.setValues(0,taperLen,95,0);
  for(int i=0;i<taperLen;i++) {
    localFlame.setColor(&flame);
    localFlame.blend(&blue,tapperPercent.Map(i));
    localFlame.blend(&black,tapperPercent.Map(i));
    lites.setPixelColor(i,&localFlame);
  }
  if (random(0,2)) {
    localFlame.blend(&yellow,30);
    lites.setPixelColor(0,&localFlame);
  }
  
  lites.show();
  delay(random(150
  ));
}
