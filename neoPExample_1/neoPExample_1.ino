#include <Adafruit_NeoPixel.h>

#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <runningAvg.h>
#include <servo.h>
#include <timeObj.h>

#include <chainPixels.h>
#include <liteLine.h>
#include <neoPixel.h>

#define NUM_LEDS  8
#define LED_PIN   8
#define ANA_PIN   A0

blinker   heartBeat;
neoPixel  lightStrip(NUM_LEDS, LED_PIN);
mapper    blendMapper(0,1023,0,100);
mapper    countMapper(0,1023,0,7);
mapper    modMapper(0,7,100,0);
colorObj  baseColor(GREEN);

void setup() {
  
  heartBeat.setBlink(true);
  lightStrip.begin();
}


void loop() {

  unsigned int  readVal;
  colorObj      aColor;
  float         percentBlend;
  byte          LEDsToLite;
  int           i;
  float         lastLEDBlend;
  
  idle();

  readVal = analogRead(ANA_PIN);
  percentBlend = blendMapper.Map(readVal);
  aColor = baseColor.mixColors(&red,percentBlend);
  aColor.blend(&black,100-percentBlend);
  LEDsToLite = countMapper.Map(readVal);
  lightStrip.setAll(&black);
  for (i=0;i<LEDsToLite;i++) {
    lightStrip.setPixelColor(i,&aColor);
  }
  lastLEDBlend = round(countMapper.Map(readVal))%7;
  aColor.blend(&black,lastLEDBlend);
  lightStrip.setPixelColor(i,&aColor);          // Cheaty trick. i holds the next value after the loop.
  lightStrip.show();
}
