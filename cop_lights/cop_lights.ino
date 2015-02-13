#include <PinChangeInt.h>
#include <RCReciver.h>

#include <blinker.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <timeObj.h>

#include <Adafruit_NeoPixel.h>

#include <colorObj.h>
#include <liteLine.h>
#include <neoPixel.h>

#include "sparkle.h"
#include "policeBar.h"
#include "runningAvg.h"
  
#define LIGHTSTICK_PIN   3
#define AUX1_PIN         4
#define MIN_PIN          1120
#define MID_PIN          1620
#define MAX_PIN          1920
#define PIN_TOLERANCE    100

reciverPin aux1(AUX1_PIN);

neoPixel lightStick(NUM_LIGHTS,LIGHTSTICK_PIN);

policeBar  theLights(&lightStick);

runningAvg smoother(6);


void setup(void) {

  //Serial.begin(9600);

  lightStick.begin();
}


void checkState(void) {

  unsigned long aux1Val;

  aux1Val = round(smoother.addData(aux1.pinResult()));
  //aux1Val = aux1.pinResult();
  //Serial.print("Val : ");Serial.print(aux1Val);Serial.println(" ");
  if (aux1Val>=(MID_PIN-PIN_TOLERANCE) && aux1Val<=(MID_PIN+PIN_TOLERANCE)) { 
      theLights.setBar(flashing);
      lightStick.show();
  } else if (aux1Val>=(MIN_PIN-PIN_TOLERANCE) && aux1Val<=(MIN_PIN+PIN_TOLERANCE)) {  
     if (theLights.getBar()!=flood) {
       theLights.setBar(flood); 
       lightStick.show();
     }
  } else {    
    /*
    if (theLights.getBar()!=litesOff) {
      theLights.setBar(litesOff);
      lightStick.show();
    }
    */
  }
}


void loop(void) {

  idle(); 
  checkState();
}



