#include "theRings.h"

colorMultiMap theColors;

starLight::starLight(void) 
  : pixelGroup(16),
  timeObj(FRAME_MS) {

  init = false;
  start(); 
}


void starLight::setupColors(void) {

  colorObj      aColor;
  
  for (int i=0;i<16;i++) {
    aColor = theColors.Map(i);
    setPixelColor(i,&aColor);
  }
}


void starLight::draw(void) {

  if (!init) {
    setupColors();
    init = true;
  }
  if (ding()) {
    start();
    roll(true);
  }
}



portLight::portLight(void) 
  : pixelGroup(16),
  timeObj(FRAME_MS) {
    
  init = false;
  start(); 
}


void portLight::setupColors(void) {

  colorObj      aColor;

  for (int i=0;i<16;i++) {
    aColor = theColors.Map(i);
    setPixelColor(15-i,&aColor);
  }
}


void portLight::draw(void) {

  if (!init) {
    setupColors();
    init = true;
  }
  if (ding()) {
    start();
    roll(false);
  }
}
