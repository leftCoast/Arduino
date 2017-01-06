#include "screen.h"
#include "adafruit_1431_Obj.h"

displayObj* screen = NULL;

#define sclk 13         //Just to get up and running. I promise!
#define mosi 11
#define dc   7
#define cs   9
#define rst  8

boolean initScreen(byte hardware, int inRotation) {

  switch (hardware) {
    case ADAFRUIT_1431 :
      screen = (displayObj*) new adafruit_1431_Obj(cs, dc, mosi, sclk, rst);
      if (screen) {
        return(screen->dispObjBegin());
      } else {
        return false;
      }
      break;
    case ADAFRUIT_1947 :
       //screen = (displayObj*) new adafruit_1947_Obj(cs, dc, mosi, sclk, rst);
      if (screen) {
        //return(screen->begin());
        //screen.initR(INITR_144GREENTAB);
        false;
      } else {
        return false;
      }
      break;
  }
}

