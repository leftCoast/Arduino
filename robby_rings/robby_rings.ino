#include <liteLine.h>
#include <neoPixel.h>
#include <blinker.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <RCReciver.h>
#include <servo.h>
#include <timeObj.h>
#include <colorObj.h>

#include <PinChangeInt.h>

#include "RCReciver.h"

#include <Adafruit_NeoPixel.h>

// ******************************************



// ******************************************

#define NUM_LEDS 12
#define LED_PIN 3

#define DARK_BLEND   95
#define LIGHT_BLEND  20

#define MAX_MS       3000
#define SLICE_MS     25

#define STARTUP_MS   8000

neoPixel lightRing(NUM_LEDS,LED_PIN);

colorMapper lightMapper;
mapper startMapper(0,STARTUP_MS,100,0);

mapper trigMapper(0,0,0,0);

timeObj lightTimer(SLICE_MS);
int currentSlice;
colorObj black(BLACK);
unsigned long startMS;

void setup(void) {

   //Serial.begin(9600);
   colorObj blue(BLUE);
   colorObj green(GREEN);
   colorObj red(RED);
   
   colorObj lightBlue = blue.blend(&red,15);
   lightBlue = lightBlue.blend(&green,5);
   
   colorObj darkBlue = blue.blend(&black,DARK_BLEND);
    lightBlue = lightBlue.blend(&black,LIGHT_BLEND);

   lightMapper.setColors(&darkBlue,&lightBlue);
   lightRing.begin();
   setRing(black);
   currentSlice = 0;
   trigMapper.setValues(0,MAX_MS,0,2*3.1416);
   lightTimer.start();
   startMS = millis();
}


void setRing(colorObj color) {

   for(int i=0;i<NUM_LEDS;i++) {
      lightRing.setPixelColor(i,&color);
   }
   lightRing.show();
}


float calculatePercent(void) {
   
   float percent;
   float value;
   
   if(currentSlice>=MAX_MS) {
      currentSlice = 0;
   }
   value = trigMapper.Map(currentSlice);
   percent = (sin(value)+1) * 50;           // sin() ranges from -1 -> 1 Adding 1 gives 0 -> 2 x 50 gives.. 0 -> 100
   currentSlice = currentSlice + SLICE_MS;
   //Serial.print("currentSlice : ");Serial.print(currentSlice);Serial.print("  Percent : ");Serial.println(percent);
   return percent;
}


void loop(void) {

   float percent;
   colorObj result;
   
   if (lightTimer.ding()) {
      percent = calculatePercent();
      result = lightMapper.Map(percent);
      result = result.blend(&black,startMapper.Map(millis()-startMS));
      setRing(result);
      lightTimer.stepTime();
   }
}





