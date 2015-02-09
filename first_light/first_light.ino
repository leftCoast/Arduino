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

#define NUM_LEDS 8
#define HALF_NUM_LEDS 4
#define LED_PIN 3
#define WAIT 33 

#define MAX_RC       1900
#define MID_RC       1500
#define MIN_RC       1100

#define THROTTLE_PIN 4
#define ROLL_PIN     5
#define PITCH_PIN    6
#define YAW_PIN      8

#define MIN_ROLL        50    // Percent value drop at full roll
#define MIN_ROLL_CUTOFF 47
#define MAX_ROLL_CUTOFF 53

Adafruit_NeoPixel theLights(NUM_LEDS,LED_PIN,NEO_GRB + NEO_KHZ800);

timeObj timer(WAIT);

reciverPin throttle(THROTTLE_PIN);
reciverPin roll(ROLL_PIN);
reciverPin yaw(YAW_PIN);
reciverPin pitch(PITCH_PIN);

mapper pinToPercent(MIN_RC,MAX_RC,0,100);

multiMap pinToAmount;

void setup(void) {

   //Serial.begin(9600);

   theLights.begin();
   timer.start();
   clearLights();
}


void clearLights(void) { setStick(BLACK); }


void setStick(colorObj color) {

   for(int i=0;i<NUM_LEDS;i++) {
      theLights.setPixelColor(i, color.getRed(), color.getGreen(), color.getBlue());
   }
   theLights.show();
}


void loop(void) {
   
   if (timer.ding()) {

      colorMapper pitchMapper(RED,GREEN);
      colorObj pitchColor(pitchMapper.Map(pinToPercent.Map(pitch.pinResult())));

      colorObj black(BLACK);
      colorMapper throttleMapper(&black,&pitchColor);

      colorObj pitchNThrottleColor(throttleMapper.Map(pinToPercent.Map(throttle.pinResult())));

      float yawVal = pinToPercent.Map(yaw.pinResult());
      colorMapper rollMapper(&pitchNThrottleColor,&black);
      colorObj darkSide(BLACK);
      float rollVal = pinToPercent.Map(roll.pinResult());
      rollVal = (rollVal+yawVal)/2;
      if ((rollVal<50 && rollVal>MIN_ROLL_CUTOFF)||(rollVal>50 && rollVal<MAX_ROLL_CUTOFF)) {
         for(int i=0;i<NUM_LEDS;i++) {
            theLights.setPixelColor(i, pitchNThrottleColor.getRed(),pitchNThrottleColor.getGreen(),pitchNThrottleColor.getBlue());
         }
      } else if (rollVal>50) {
         mapper rollAmount(50,100,MIN_ROLL,100);
         darkSide = rollMapper.Map(rollAmount.Map(rollVal));
          for(int i=0;i<HALF_NUM_LEDS;i++) {
            theLights.setPixelColor(i, darkSide.getRed(),darkSide.getGreen(),darkSide.getBlue());
         }
        
         for (int i=HALF_NUM_LEDS;i<NUM_LEDS;i++) {
           theLights.setPixelColor(i, pitchNThrottleColor.getRed(),pitchNThrottleColor.getGreen(),pitchNThrottleColor.getBlue());
         }
      } else {
         mapper rollAmount(50,0,MIN_ROLL,100);
         darkSide = rollMapper.Map(rollAmount.Map(rollVal));
         for(int i=0;i<HALF_NUM_LEDS;i++) {
            theLights.setPixelColor(i, pitchNThrottleColor.getRed(),pitchNThrottleColor.getGreen(),pitchNThrottleColor.getBlue());
         }
         
         for (int i=HALF_NUM_LEDS;i<NUM_LEDS;i++) {
           theLights.setPixelColor(i, darkSide.getRed(),darkSide.getGreen(),darkSide.getBlue());
         }   
      }
     
      theLights.show();
      timer.stepTime();
   }
}




