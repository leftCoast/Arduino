
#include "policeBar.h"


policeBar::policeBar(neoPixel* inLights) { 

  lights = inLights;
  for (int i=0;i<NUM_LIGHTS;i++) {                // For every llight..
    lights->setPixelColor(i,&black);              // Starting with black.
    lightList[i] = new sparkle(lights,i,-1);      // Here's your pixel drivers and your always on.
    lightList[i]->setColor(&black);               // Can reset this later.              
    lightList[i]->hookup();
  }
  state = litesOff;
}


policeBar::~policeBar(void) {
  
   for (int i=0;i<NUM_LIGHTS;i++) {                // For every llight..
    delete lightList[i];
    lightList[i] = NULL;
  }
}


colorObj policeBar::chooseColor(void) {

  long val = random(0,11);
  switch (val) {
  case 0 : 
  case 1 :
  case 2 :
  case 3 : return red; 
  case 4 :
  case 5 :
  case 6 :
  case 7 : return blue;
  case 8 :  return white;
  case 9 :
  case 10 : return black;  
  }
}


void  policeBar::setBar(lightStates inState) { 

  if(inState!=state) {
    switch (inState) {
    case litesOff : 
      for (int i=0;i<NUM_LIGHTS;i++) {                // For every llight..
        lightList[i]->fade();              
      }
      state = litesOff;
      break;
    case flashing :
      for (int i=0;i<NUM_LIGHTS;i++) {                // For every llight..
        lightList[i]->setColor(&(chooseColor()));
        lightList[i]->setTime(random(TIME_LOW,TIME_HIGH));
        lightList[i]->flash();
      }
      state = flashing;
      break;
    case flood :
      for (int i=0;i<NUM_LIGHTS;i++) {                // For every llight..
        lightList[i]->setColor(&white);
        lightList[i]->setTime(-1);
        lightList[i]->flash();
      }
      state = flood;
      break;
    }
  }
}


lightStates policeBar::getBar(void) { return state; }


void policeBar::idle(void) {

  if (flashing) {
    for (int i=0;i<NUM_LIGHTS;i++) {                // For every llight..
      if (!lightList[i]->getState()) {
        lightList[i]->setColor(&(chooseColor()));
        lightList[i]->setTime(random(TIME_LOW,TIME_HIGH));
        lightList[i]->flash();
      }
    }
  }
}
