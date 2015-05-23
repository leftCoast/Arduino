#include <idlers.h>
#include <timeObj.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

#include "sprit.h"
#include "animations.h"
#include "shakeNTinkle.h"

#define MIN_SHAKE     5      // Threshold vibration value for trigering reset of the sleep timer.
#define MIN_TINKLE    20     // currently ignored
#define WAKE_TIME     45000  // Ms before dimming the lights some
#define RUN_TIME      65000  // Ms to time out to sleep mode.
#define SUM_TIME      50     // Ms to gather shakes and tinkles.
#define NUM_DATA      5      // Data points per.
#define FULL_BRIGHT   15     // 15 is the max brightness setting.
#define DIM_LIGHT     2      // Dim light setting
#define ANIME_TIME    750    // Ms between checking to see if running an animation is a good idea.
#define ANIME_ODDS    50      // Chances in 100 that an animation will get played.

#define SENS1_SWITCH_PIN 3
#define SENS2_SWITCH_PIN 4

shakeNTinkle sensors(SENS1_SWITCH_PIN, SENS2_SWITCH_PIN,SUM_TIME,NUM_DATA);

#define KID_NAME " Ella"
#define KID_FOOD " Chicken & Noodles!!"

Adafruit_8x8matrix matrix;

timeObj  animeTimer(ANIME_TIME);
timeObj  sleepTimer(RUN_TIME);
timeObj  drowsyTimer(WAKE_TIME);

thinkSprit  background(&matrix);
smileSprit  theSmileSprit(&matrix);
eyesSprit   theEyesSprit(&matrix);
heartSprit  theHeartSprit(&matrix);
starSprit   theStarSprit(&matrix);
twelveSprit theTwelveSprit(&matrix);

text8x8    nameStr(&matrix, KID_NAME, 75);
text8x8    foodStr(&matrix, KID_FOOD, 75);

sprit* currentSprit = NULL;

boolean  awake;
boolean  lightState;

void setup() {

  sensors.begin();
  matrix.begin(0x70);              // pass in the address
  matrix.setRotation(2);
  awake = true;
  lightState = LOW;                // So the next call will work.
  lowLight(false);
  startBackground();
}


void startSprit(sprit* spritPtr) {

  background.stopSprit();
  currentSprit = spritPtr;
  currentSprit->startSprit();
}


void startBackground(void) {
  
  startSprit(&background);        // Restart the background blinking
  animeTimer.start();             // Restart chekcing to run animations.
}


boolean runningSprit(void) { return currentSprit != &background; }


void sleep(void) {
  
  currentSprit->stopSprit();
  matrix.clear();
  matrix.writeDisplay();
  awake = false;
}


void wakeUp(void) {
  
  startBackground();
  awake = true;
}


void lowLight(boolean makeLow) {
  
  
  if (makeLow && lightState != LOW) {
    matrix.setBrightness(DIM_LIGHT);
    lightState = LOW;
  } else if (!makeLow && lightState != HIGH) {
    matrix.setBrightness(FULL_BRIGHT);
    lightState = HIGH;
    drowsyTimer.start();
  }
}


boolean trySprit() {

  int rNum = random(0, 101); 
  return rNum <= ANIME_ODDS;
}


void chooseAnimation(void) {
  
  int rNum = random(0, 7);
  switch (rNum) {
    case 0 :  startSprit(&nameStr); break;
    case 1 :  startSprit(&foodStr); break;
    case 2 :  startSprit(&theSmileSprit); break;
    case 3 :  startSprit(&theHeartSprit); break;
    case 4 :  startSprit(&theEyesSprit); break;
    case 5 :  startSprit(&theStarSprit); break;
    case 6 :  startSprit(&theTwelveSprit); break;
  } 
}


void loop(void) {

  idle();                               // Eeryone gets some time to do their stuff.
  if (awake) {                          // If were active..
    if (runningSprit()) {               // if were running an animation.
      if (!currentSprit->active()) {    // And its over..
        startBackground();              // Restart chekcing to run animations.
      }
    } else if (animeTimer.ding()) {     // Not drowsy, not running an animation, is it time to roll the dice?
      if (trySprit()) {                 // Roll the dice and see if we "win" an animation.
        chooseAnimation();              // You win! Choose your animation.
      } else { 
        animeTimer.stepTime();            // Reset the timer for next check. 
      }
    }
     if (drowsyTimer.ding()){            // But getting close to shutdown..
      lowLight(true);                   // Low light is a signal that were going to close shop in a bit.
    }
    if (sleepTimer.ding()) {            // If we been still for too long..
      sleep();                          // Shut down the lights to save power.
    }      
  }
  if (sensors.getShake() > MIN_SHAKE) { // In any case, see if someine is shaking this thing.
    if (!awake) {                       // Been shook, if asleep..
      wakeUp();                         // Wake up!
    }
    lowLight(false);                    // Make the lights bright!
    sleepTimer.start();                 // Restart bed time timer.
  } 
}


