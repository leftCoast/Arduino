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
  fourColorRing.setPixelColor(5,&aColor);
  
  fourColorRing.show();
  
  threeColorStick.begin();
  threeColorStick.setPixelColor(0,&black);
  threeColorStick.setPixelColor(1,&yellow);
  threeColorStick.setPixelColor(2,&aColor);
  threeColorStick.setPixelColor(7,&blue);
  threeColorStick.show();
  delay(250);
  aColor = threeColorStick.getPixelColor(0);
  
  for(int i=0;i<640;i++) {
    shiftPixels(&fourColorRing,true);
    shiftPixels(&threeColorStick,true);
    aColor = randomColor();
    fourColorRing.setPixelColor(0,&aColor);
    aColor = randomColor();
    threeColorStick.setPixelColor(0,&aColor);
    fourColorRing.show();
    threeColorStick.show();
    delay(50);
  }

}

colorObj randomColor(void) {

  colorObj aColor;
  aColor.setColor((byte)random(0,256),(byte)random(0,256),(byte)random(0,256));
  aColor.blend(&black,80);
  return aColor;
}


void shiftPixels(neoPixel* pixels,bool toEnd) {

  colorObj  aColor;
  int       last;

  last = pixels->numPixels()-1;
  if (toEnd) {                              // Feed new colors onto pixel 0, pull 'em through to the end.
    for(int i=last;i>0;i--) {
      aColor = pixels->getPixelColor(i-1);
      pixels->setPixelColor(i,&aColor);
    }
  } else {                                  // Feed new colors onto last pixel, push 'em to the start..
    for(int i=0;i<last;i++) {
      aColor = pixels->getPixelColor(i+1);
      pixels->setPixelColor(i,&aColor);
    }
  }
}


void loop() {
  // put your main code here, to run repeatedly:

}
