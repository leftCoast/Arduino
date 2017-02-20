#include <Adafruit_NeoPixel.h>

#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <runningAvg.h>
#include <timeObj.h>

#include <chainPixels.h>
#include <liteLine.h>
#include <neoPixel.h>

#define NUM_PIXELS  16
#define RING_PIN    2

neoPixel fourColorRing(NUM_PIXELS,RING_PIN,NEO_GRBW + NEO_KHZ800);
neoPixel threeColorStick(8,7);

void setup() {

  //Serial.begin(9600);
  
  colorObj aColor;

  aColor.setColor(&red);
  aColor.blend(&blue,50);
  
  
 
  fourColorRing.begin();
  fourColorRing.show();
  fourColorRing.setPixelColor(0,&white);
  fourColorRing.Adafruit_NeoPixel::setPixelColor((uint16_t)1,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)255);
  fourColorRing.setPixelColor(2,&white);
  fourColorRing.setPixelColor(3,&yellow);
  fourColorRing.setPixelColor(4,&aColor);
  fourColorRing.setPixelColor(5,&blue);
  fourColorRing.setPixelColor(6,&aColor);
  fourColorRing.show();
  
  threeColorStick.begin();
  threeColorStick.setPixelColor(0,&black);
  threeColorStick.setPixelColor(1,&yellow);
  threeColorStick.setPixelColor(2,&aColor);
  threeColorStick.setPixelColor(7,&blue);
  
  threeColorStick.show();
  delay(250);
  aColor = threeColorStick.getPixelColor(0);
}

colorObj randomColor(void) {

  colorObj aColor;
  aColor.setColor((byte)random(0,256),(byte)random(0,256),(byte)random(0,256));
  aColor.blend(&black,80);
  return aColor;
}



void loop() {
  
  fourColorRing.roll(false);
  fourColorRing.show();
  delay(100);

}
