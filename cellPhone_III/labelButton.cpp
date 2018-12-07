#include "labelButton.h"
#include  "litlOS.h"

labelButton::labelButton(char* inLabel,word locX, word locY,byte width,byte height) 
  : label(locX,locY,width-1,height-1,inLabel) { clickable(true); }


labelButton::~labelButton(void) { }


// Default is a rectangle with drop-shadow in system colors.
// this is just an example on how to do it. You can do whatever you like.
void labelButton::drawSelf(void) {

  colorObj baseColor = ourOS.getColor(SYS_CONTROL_BTN_COLOR);
  colorObj textColor = ourOS.getColor(SYS_CONTROL_LBL_COLOR);
  colorObj shadowColor = ourOS.getColor(SYS_SHADOW_COLOR);
                                                  
  screen->drawRect(x+1, y+1, width-1, height-1, &shadowColor);
  if (clicked) {
    screen->fillRect(x, y, width-1, height-1, &white);
  } else {
    screen->fillRect(x, y, width-1, height-1, &baseColor);
  }
  
  setTextSize(2);
  setJustify(TEXT_CENTER);
  setColors(&textColor);
  x = x+4;
  y = y+2;
  label::drawSelf();
  x = x-4;
  y = y-2;
}


// You been clicked, do aomething.
void labelButton::doAction(void) { Serial.print(buff); }
