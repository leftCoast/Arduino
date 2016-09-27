#include <Adafruit_NeoPixel.h>

#include <blinker.h>
#include <colorObj.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <servo.h>
#include <timeObj.h>

#include <chainPixels.h>
#include <liteLine.h>
#include <neoPixel.h>

#include "shakeNTinkle.h"

#define MIN_SHAKE     10      // Threshold vibration value for trigering reset of the sleep timer.
#define MORE_SHAKE    20      // Really shaking its head!
#define MIN_TINKLE    20     // currently ignored
#define SENS1_SWITCH_PIN 3
#define SENS2_SWITCH_PIN 4
#define SUM_TIME      40     // Ms to gather shakes and tinkles.
#define NUM_DATA      5      // Data points per.
#define FAST_DELAY    25     // How long to hold a white blink.
#define DARK_PERCENT  75

colorObj baseColor;
blinker aBLinker;
neoPixel lightBar(8, 8);
shakeNTinkle sensors(SENS1_SWITCH_PIN, SENS2_SWITCH_PIN, SUM_TIME, NUM_DATA);
long randomFlashNum1;
long randomFlashNum2;
timeObj fastTimer(FAST_DELAY);

void setAll(colorObj* color) {

  for (byte i = 0; i <= 7; i++) {
    lightBar.setPixelColor(i, color);
  }
  lightBar.show();   // Lest show it!
}

void setup() {

  sensors.begin();
  baseColor.setColor(&blue);
  colorObj temp = baseColor.blend(&black, DARK_PERCENT);
  baseColor.setColor(&temp);
  lightBar.begin();
  aBLinker.hookup();
  aBLinker.setBlink(false); // Set true for sanity checks.
  setAll(&baseColor);

}

void flash(int inNum, colorObj* flashColor) {

  //lightBar.setPixelColor(inNum,flashColor);
  if (bitRead(randomFlashNum1, inNum) && bitRead(randomFlashNum1, inNum)) {
    lightBar.setPixelColor(inNum, flashColor);
  }
}


void shake1(void) {


  flash(3, &white);
  flash(4, &white);
  lightBar.show();   // Lest show it!
  fastTimer.start();
  while (!fastTimer.ding()) idle();
  setAll(&baseColor);

  flash(2, &white);
  flash(5, &white);
  lightBar.show();   // Lest show it!
  fastTimer.start();
  while (!fastTimer.ding()) idle();
  setAll(&baseColor);

  flash(1, &white);
  flash(6, &white);
  lightBar.show();   // Lest show it!
  fastTimer.start();
  while (!fastTimer.ding()) idle();
  setAll(&baseColor);

  flash(0, &white);
  flash(7, &white);
  lightBar.show();   // Lest show it!
  fastTimer.start();
  while (!fastTimer.ding()) idle();

  setAll(&baseColor);   // Show's over, reset the lights.
}

void loop() {

  idle();
  if (sensors.getShake() > MIN_SHAKE) {
    randomFlashNum1 = random(0, 256);
    randomFlashNum2 = random(0, 256);
    shake1();
  } 
  if (sensors.getShake() > MORE_SHAKE) {
    randomFlashNum1 = random(0, 256);
    randomFlashNum2 = random(0, 256);
    shake1();
  }
}


