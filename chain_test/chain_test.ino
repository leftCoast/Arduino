#include <Adafruit_NeoPixel.h>

#include <blinker.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <servo.h>
#include <timeObj.h>

#include <colorObj.h>

#include <liteLine.h>
#include <neoPixel.h>
  
#include "chainPixels.h"
#include "robbyRing.h"

#define NUM_LEDS 12
#define LED_PIN 3

chainPixels theChain(LED_PIN);
robbyRing* ring1;
  
  
void setup() {
  
  //Serial.begin(9600);
  //delay(10000);  // 10 seconds to get screen up.
  
  ring1 = new robbyRing(NUM_LEDS);
  theChain.addGroup(ring1);
  theChain.hookup();
}

void loop() { idle(); }
