
#include "sparkle.h"




sparkle::sparkle(neoPixel* inLights,word inLoc,float inTime) {

  lights = inLights;
  loc    = inLoc;
  lightOn = false;
  setTime(inTime);  
}


sparkle::~sparkle(void) {  
}


void sparkle::setColor(colorObj* inColor) {

  if (lightOn)
    fade();
  color.setColor(inColor);
}


void sparkle::setTime(float inTime) {

  if (lightOn) {
    fade();
  }
  if (inTime<0) {
    alwaysOn = true;
  } 
  else {
    alwaysOn = false;
    onTime.setTime(inTime);
  }
}


boolean sparkle::getState(void) { 
  return lightOn; 
}


void sparkle::flash(void) { 

  savedColor = lights->getPixelColor(loc);
  lights->setPixelColor(loc,&color);
  onTime.start();
  lightOn = true;
}


void sparkle::fade(void) { 

  lights->setPixelColor(loc,&savedColor);
  lightOn = false;
}


void sparkle::idle(void) {

  if (lightOn && !alwaysOn && onTime.ding()) {
    fade();
  } 
}


