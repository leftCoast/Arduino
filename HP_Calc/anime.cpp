#include "anime.h"

#define SHINE_COLOR   BLUE    // What color we gonna shine with?
#define SHINE_WIDTH   24       // +/- SHINE_WIDTH is the blend distance.
#define SHINE_PERCENT 70       // How much white do we want?


anime::anime(word inX,word inY,word inWidth,word inHeight,float inTime) :
            drawObj(inX,inY,inWidth,inHeight),
            timeObj(inTime) { offset = 0; }
            

void anime::begin(void) {

  word      endX;
  word      shineCenter;
  colorObj  endColor(BLUE);
  colorObj  shine(SHINE_COLOR);
  
  endX = locX+(width*2);
  shineCenter = endX-SHINE_WIDTH;
  endColor.blend(&black,80);
  shine.blend(&white,SHINE_PERCENT);               // Add in the shine
  addColor(locX,&endColor);                        // Start dark..
  addColor(endX,&endColor);                        // End dark..
  addColor(shineCenter - SHINE_WIDTH,&endColor);   // Start dark..
  addColor(shineCenter,&shine);                    // In the middle? We SHINE!
  endColor.setColor(&white);
  endColor.blend(&black,70);
  screen->drawRect(locX,locY,width,height,&endColor);
  hookup();
  start();
  needRefresh = true;
}


colorObj anime::calcColor(word inX) {

  colorObj  temp;
  word      index;
  word      maxX;
  long      rNum;
  
  rNum = random(75);                    // Sometimes we just pass back white.
  if (rNum==10) {
    temp.setColor(&white);
    return temp;
  }
  index = inX+offset;
  maxX = locX+(width*2);
  if(index>maxX) index = index - maxX;
  temp = Map(index);
  return temp;
}


void anime::drawSelf(void) {

  colorObj  tempColor;
  word       startX;
  word       endX;
  word       top;
  word       lineH;
  
  startX = locX+1;
  endX = locX+width-2;
  top = locY+1;
  lineH = height-2;
  for (int i=startX;i<=endX;i++) {
    tempColor = calcColor(i);
    screen->drawVLine(i,top,lineH,&tempColor);
  }
  offset++;
  if(offset>(width*2)) offset = 0;
}


void anime::idle(void) {

  if (ding()) {
    needRefresh = true;
    start();            // Doesn't matter if we slip some.
  }
}


