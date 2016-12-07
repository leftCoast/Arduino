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

#define SHAKE_TIME  10  // How long in ms to sample data.
#define SHAKE_AVE   5   // How many data points to average over
#define SHAKE_PIN   3   // The pin the shake sensor is hooked to.

#define SHAKE_MAX   5  // Cutoff for saying there's a shake going on.

neoPixel theLights(NUM_LEDS,LED_PIN);
shaker  theShaker(SHAKE_PIN,SHAKE_TIME,SHAKE_AVE);

byte count;
void setup() {

    theLights.begin();
    theShaker.begin();
    //count = 0;
}


void loop() {

  idle();
  if (theShaker.result()>SHAKE_MAX) {
    theLights.setAll(&red);
  }
  else {
    theLights.setAll(&green);
  }
  theLights.show();
  delay(100);
}
