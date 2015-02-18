#include <PinChangeInt.h>
#include <RCReciver.h>

#include <Adafruit_NeoPixel.h>

#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <timeObj.h>
#include <runningAvg.h>
#include <colorObj.h>
#include <neoPixel.h>
  
#include <chainPixels.h>
//#include "robbyRing.h"
#include "policeBar.h"

//#define NUM_RING_LEDS   12
//#define NUM_STICK_LEDS  8

#define LED_PIN   3
#define AUX1_PIN  4
#define AUX2_PIN  5

#define MIN_PIN          1120
#define MID_PIN          1620
#define MAX_PIN          1920
#define PIN_TOLERANCE    100
#define AVERAGE_NUM      5

chainPixels      theChain(LED_PIN);     // Create the chain to manage the LEDs.
reciverPin       aux1(AUX1_PIN);
reciverPin       aux2(AUX2_PIN);
runningAvg       smoother(AVERAGE_NUM);
policeBar*       lightStick;

void setup() {
  
  //Serial.begin(9600);
  lightStick = new policeBar();
  lightStick->blinkOn();
  theChain.addGroup(lightStick);
  
  theChain.hookup();
  
}

void loop() {

  unsigned long aux1Val;
  unsigned long aux2Val;
  
  idle();
  
  aux1Val = round(smoother.addData(aux1.pinResult()));
  aux2Val = round(smoother.addData(aux2.pinResult()));
  if (aux2Val>(MID_PIN-PIN_TOLERANCE)) {
    lightStick->blinkOn(); 
  } else if (aux1Val>(MID_PIN-PIN_TOLERANCE)) { 
      lightStick->lightOff();
  } else {
     lightStick->floodOn(); 
  }
}

