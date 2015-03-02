#include <Adafruit_NeoPixel.h>
#include <chainPixels.h>
#include <liteLine.h>
#include <neoPixel.h>

#include <blinker.h>
#include <idlers.h>
#include <lists.h>
#include <mapper.h>
#include <multiMap.h>
#include <PulseOut.h>
#include <RCReciver.h>
#include <servo.h>
#include <timeObj.h>
#include <colorObj.h>

#include <PinChangeInt.h>

#include "RCReciver.h"


#include "RCTaillight.h"



// ******************************************


class floodLight : public pixelGroup {

  public :

    floodLight(void);
    ~floodLight(void);

    void setFlood(boolean onOff);
    virtual void       draw(void);

  protected :
    boolean nextState;
    boolean isOn;

};

floodLight::floodLight(void) : pixelGroup(8) {

  nextState = false;
  isOn = false;
}


floodLight::~floodLight(void) { }


void floodLight::setFlood(boolean onOff) {
  nextState = onOff;
}


void floodLight::draw(void) {

  if (nextState != isOn) {
    if (nextState) {
      setPixels(&white);
    } else {
      setPixels(&black);
    }
    isOn = nextState;
  }
}

// ***************************



#define WAIT 33


#define MAX_RC       1900
#define MIN_RC       1100
#define MAX_FLOOD    1800

#define LED_PIN      8
#define THROTTLE_PIN 3
#define ROLL_PIN     4
#define PITCH_PIN    5
#define YAW_PIN      6
#define FLOOD_PIN    12

reciverPin yawPin(YAW_PIN);
reciverPin throttlePin(THROTTLE_PIN);
reciverPin rollPin(ROLL_PIN);
reciverPin pitchPin(PITCH_PIN);
reciverPin floodPin(FLOOD_PIN);

timeObj timer(WAIT);
chainPixels  theChain(LED_PIN);
RCTaillight* theTailLight;
floodLight*  flood1;
floodLight*  flood2;

void setup(void) {

  //Serial.begin(9600);
  theTailLight = new RCTaillight(MIN_RC,  // leftYaw
      MAX_RC,  // rightYaw
      MIN_RC,  // throttleLow
      MAX_RC,  // throttleHi
      MIN_RC,  // rollPort
      MAX_RC,  // rollStarboard
      MAX_RC,  // pitchDn
      MIN_RC,  // pitchUp
      false);  // reverse
  theChain.addGroup(theTailLight);
  
  flood1 = new floodLight();
  theChain.addGroup(flood1);
  
  flood2 = new floodLight();
  theChain.addGroup(flood2);
  
  theChain.hookup();
  timer.start();
}

void loop(void) {

  int yaw;
  int throttle;
  int roll;
  int pitch;
  boolean floodOnOff;
  
  idle();
  if (timer.ding()) {                                  // Time's up! Lets go to work.
      
      yaw = yawPin.pinResult();                        // Ok, time to refresh. Check the reciever pins.
      throttle = throttlePin.pinResult();
      roll = rollPin.pinResult();
      pitch = pitchPin.pinResult();
      floodOnOff = floodPin.pinResult() < MAX_FLOOD;
      
      theTailLight->setValues(yaw,throttle,roll,pitch);  // Stuff the results into the lightbars.
      flood1->setFlood(floodOnOff);
      flood2->setFlood(floodOnOff);
      timer.stepTime();
  }
}




