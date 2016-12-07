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

#include "shaker.h"

#define NUM_LEDS  32
#define LED_PIN   8

#define SHAKE_TIME  250  // How long in ms to sample data.
#define SHAKE_AVE   5   // How many data points to average over
#define SHAKE_PIN   3   // The pin the shake sensor is hooked to.

#define SHAKE_MAX1   3  // Cutoff for saying there's a shake going on.
#define SHAKE_MAX2   100  // Cutoff for saying there's a BIG shake going on.

#define CHECK_TIME   50  // How often to check the shake sensor.

neoPixel theLights(NUM_LEDS, LED_PIN);
shaker  theShaker(SHAKE_PIN, SHAKE_TIME, SHAKE_AVE);

timeObj checkTimer(CHECK_TIME);

colorMultiMap theColorMapper;

void setup() {

  theLights.begin();
  theShaker.begin();
  theColorMapper.addColor(0,&green);
  theColorMapper.addColor(SHAKE_MAX1,&green);
  theColorMapper.addColor(SHAKE_MAX2,&red);
  checkTimer.start();
  theLights.setAll(&blue);
}


void loop() {

  idle();

  if (checkTimer.ding()) {
    checkTimer.stepTime();
    colorObj aColor = theColorMapper.Map(theShaker.result());
    theLights.setAll(&aColor);
    theLights.show();
  }
}
