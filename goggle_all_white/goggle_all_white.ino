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

#include <neoPixel.h>

neoPixel rings(32,3);

void setup() {
  rings.begin();
  rings.setAll(&white);
  rings.show();
}

void loop() {
  // put your main code here, to run repeatedly:

}
