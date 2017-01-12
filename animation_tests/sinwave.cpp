#include "sinwave.h"

#define DEF_FREQ  10

sinwave::sinwave(word inX,word inY,word width,word height,boolean inClicks)
  :drawObj(inX,inY,width,height,inClicks) {
    
  freq = DEF_FREQ;          // Just some nice defaults to get going with..
  lColor.setColor(0,0,0);
  transparent = true;
  needRefresh = true;  
}

void sinwave::setFreq(float pixelsPerCycle) {

  freq = pixelsPerCycle;
  needRefresh = true;
}


void sinwave::setColors(colorObj* inLColor,colorObj* inBColor) {

  lColor.setColor(inLColor);
  if (inBColor) {
    bColor.setColor(inBColor);
    transparent = false;
  } else {
    transparent = true;
  }
}


void sinwave::drawSelf(void) {

  int y;
  float     stepSize;
  float     amplitude;
  
  
  stepSize = (2* M_PI)/freq;
  amplitude = height/2;
  if (transparent) {
    //screen->drawRect(locX,locY,width,height,&green);
    for(word x=0;x<width;x++) {
      y = round(sin(x*stepSize) * amplitude);
      screen->drawPixel(x+locX,locY-y+amplitude,&lColor);
    }
  } else {
    
    colorObj  blurColor[2];
    float     fResult;
    float     percentNext;
    float     percentOrig;
    //screen->fillRect(locX,locY,width,height,&bColor);
    for(word x=0;x<width;x++) {
      fResult = sin(x*stepSize) * amplitude;
      percentNext = (fResult - trunc(fResult)) * 100;
      percentOrig = 100 - percentNext;
      blurColor[0].setColor(&bColor);
      blurColor[1].setColor(&bColor);
      blurColor[0].blend(&lColor,percentOrig);
      blurColor[1].blend(&lColor,percentNext);
      //Serial.print("res: ");Serial.print(fResult);Serial.print("  org: ");Serial.print(percentOrig);Serial.print("  next: ");Serial.println(percentNext);
      y = trunc(fResult);
      screen->drawPixel(x+locX,locY-y+amplitude,&(blurColor[0]));
      screen->drawPixel(x+locX,locY-y+amplitude-1,&(blurColor[1]));
    }
  }
}

