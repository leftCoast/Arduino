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
#include <neoPixel.h>

#include "theRings.h"

chainPixels theRings(3);
starLight ring1;
portLight ring2;


void setup() {

  setUpPattern();             // First is to set up the pattern we want.
  theRings.addGroup(&ring1);
  theRings.addGroup(&ring2);
  theRings.hookup();
}


void setUpPattern(void) {

  colorObj      aColor;

  aColor.setColor(&white);
  theColors.addColor(0,&aColor);

  aColor.setColor(&red);
  theColors.addColor(1,&aColor);

  aColor.setColor(&red);
  aColor.blend(&black,80);
  theColors.addColor(3,&aColor);

  aColor.setColor(&green);
  aColor.blend(&black,80);
  theColors.addColor(4,&aColor);

  aColor.setColor(&black);
  theColors.addColor(9,&aColor);
}


void loop() { idle(); }
