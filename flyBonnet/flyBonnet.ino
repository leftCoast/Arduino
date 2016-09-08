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

#define MIN_SHAKE     15      // Threshold vibration value for trigering reset of the sleep timer.
#define MIN_TINKLE    20     // currently ignored
#define SENS1_SWITCH_PIN 3
#define SENS2_SWITCH_PIN 4
#define SUM_TIME      40     // Ms to gather shakes and tinkles.
#define NUM_DATA      5      // Data points per.
#define FAST_DELAY    15     // How long to hold a white blink.

colorObj baseColor;
blinker aBLinker;
neoPixel lightBar(8,8);
shakeNTinkle sensors(SENS1_SWITCH_PIN, SENS2_SWITCH_PIN,SUM_TIME,NUM_DATA);

void setAll(colorObj* color) {

  for(byte i=0;i<=7;i++) {
    lightBar.setPixelColor(i,color);
  }
  lightBar.show();   // Lest show it!
}

void setup() {

  sensors.begin();
  baseColor.setColor(&blue);
  colorObj temp = baseColor.blend(&black,90);
  baseColor.setColor(&temp);
  lightBar.begin();
  aBLinker.hookup();
  aBLinker.setBlink(false); // Set true for sanity checks.
  setAll(&baseColor);

}

void shake1(void) {

  lightBar.setPixelColor(3,&white);
  lightBar.setPixelColor(4,&white);
  lightBar.show();   // Lest show it!
  delay(FAST_DELAY);
  lightBar.setPixelColor(2,&white);
  lightBar.setPixelColor(5,&white);
  lightBar.show();   // Lest show it!
  delay(FAST_DELAY);
  lightBar.setPixelColor(1,&white);
  lightBar.setPixelColor(6,&white);
  lightBar.show();   // Lest show it!
  delay(FAST_DELAY);
  lightBar.setPixelColor(0,&white);
  lightBar.setPixelColor(7,&white);
  lightBar.show();   // Lest show it!
  delay(FAST_DELAY);
  setAll(&baseColor);
}

void loop() {
  idle();
  if (sensors.getShake() > MIN_SHAKE) {
    shake1();
  }
}


