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

#include <shaker.h>

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

enum modetype {light, extreme};
modetype mode;

void setup() {

  theLights.begin();
  theShaker.begin();
  theColorMapper.addColor(0, &red);
  theColorMapper.addColor(SHAKE_MAX1, &red);
  theColorMapper.addColor(SHAKE_MAX2, &yellow);
  mode = light;
  checkTimer.start();
}


void loop() {

  float shakeState;
  idle();

  if (checkTimer.ding()) {
    colorObj aColor;

    checkTimer.stepTime();
    shakeState = theShaker.result();
    if (shakeState > SHAKE_MAX2) {
      mode = extreme;
    }
    switch (mode) {
      case light :
        aColor = theColorMapper.Map(shakeState);
        theLights.setAll(&aColor);
        theLights.show();
        break;
      case extreme :
        if (count >= 0 && count < 3) {
          colorStep(&white);
        } else {
          colorStep(&red);
        }
        count++;
        if (count >= 7) {
          count = 0;
        }
        break;
    }
  }
}
