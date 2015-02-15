#include <Adafruit_NeoPixel.h>

#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <timeObj.h>
#include <colorObj.h>
#include <neoPixel.h>
  
#include <chainPixels.h>
#include "robbyRing.h"
#include "policeBar.h"

#define NUM_RING_LEDS   12
#define NUM_STICK_LEDS  8
#define LED_PIN 3

chainPixels      theChain(LED_PIN);     // Create the chain to manage the LEDs.

void setup() {
  
  robbyRing* ring;                      // Pointer to a pixelGroup derived object.
  ring = new robbyRing(NUM_RING_LEDS);  // Create a new robbyRing object.
  theChain.addGroup(ring);              // Hand it off to the chainPixels object. 
                                        // If for some reason the chainPixels object
                                        // is told to let it go, it will delete it from memory.
  /*
  robbyTailLight*  lightStick;
  lightStick = new robbyTailLight(NUM_STICK_LEDS);
  theChain.addGroup(lightStick);
  */
  
  policeBar*  lightStick;
  lightStick = new policeBar();
  lightStick->blinkOn();
  //lightStick->floodOn();
  theChain.addGroup(lightStick);
  
  theChain.hookup();
  
}

void loop() { idle(); }
