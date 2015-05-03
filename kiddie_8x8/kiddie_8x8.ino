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
#define RUN_TIME      15000  // Ms to time out to sleep mode.
#define SUM_TIME      50     // Ms
#define NUM_DATA      5      // data points per.

#define SENS1_SWITCH_PIN 3
#define SENS2_SWITCH_PIN 4

shakeNTinkle sensors(SENS1_SWITCH_PIN, SENS2_SWITCH_PIN,SUM_TIME,NUM_DATA);

#define KID_NAME " Kayla"
#define KID_FOOD " Cheeseburger!!"

Adafruit_8x8matrix matrix;

timeObj  animeTimer(750);
timeObj  sleepTimer(RUN_TIME);

thinkSprit background(&matrix);
smileSprit theSmileSprit(&matrix);
eyesSprit  theEyesSprit(&matrix);
heartSprit theHeartSprit(&matrix);
text8x8    nameStr(&matrix, KID_NAME, 75);
text8x8    foodStr(&matrix, KID_FOOD, 75);

sprit* currentSprit = NULL;

boolean  awake;

void setup() {

  sensors.begin();
  matrix.begin(0x70);  // pass in the address
  matrix.setRotation(2);
  awake = true;
  startBackground();
}


void startSprit(sprit* spritPtr) {

  background.stopSprit();
  currentSprit = spritPtr;
  currentSprit->startSprit();
}


void startBackground(void) {

  startSprit(&background);
  animeTimer.start();
}


boolean runningSprit(void) {
  return currentSprit != &background;
}


boolean trySprit() {

  int rNum;

  rNum = random(0, 100);
  if (rNum == 0) {
    startSprit(&nameStr);
  } else if (rNum == 25) {
    startSprit(&foodStr);
  } else if (rNum == 10) {
    startSprit(&theSmileSprit);
  } else if (rNum == 40) {
     startSprit(&theHeartSprit);
  } else if (rNum == 30) {
    startSprit(&theEyesSprit);
  }
  return runningSprit();
}


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

 
/*
void showSleepTimer(void) {
  
  int val = round(sleepTimer.getMappedTime());
  for (byte j = 0; j < 8; j++) {
      if (val>=j) {
        matrix.drawPixel(0, j, LED_ON);
      } else {
        matrix.drawPixel(0, j, LED_OFF);
      }
  }
  matrix.writeDisplay();
} 
*/

void loop(void) {

  idle();
  if (awake) {
    if (sleepTimer.ding()) {
      sleep();
    } else if (runningSprit()) {
      if (!currentSprit->active()) {
        startBackground();
      }
    } else if (animeTimer.ding()) {
      if (!trySprit()) {
        animeTimer.stepTime();
      }
    }
  }
  if (sensors.getShake() > MIN_SHAKE) {
    if (!awake) {
      wakeUp();
    }
    sleepTimer.start();
  } 
}


