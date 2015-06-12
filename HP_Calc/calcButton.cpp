#include "calcButton.h"
#include <calculator.h>

extern calculator mCalc;
extern boolean    buttonPressed;

calcButton::calcButton(char* inFStr,word locX, word locY,byte width, byte height) 
  : touchObj(locX,locY,width,height) {
    
    int numChars = strlen(inFStr) + 1;
    fStr = (char*) malloc(numChars);
    strcpy(fStr,inFStr);
  }
  
void calcButton::drawSelf(void) {
  
    colorObj activeColor(white);
    activeColor.blend(&black,30);
    colorObj inactiveColor(white);
    activeColor.blend(&black,90);
    colorObj touchedColor(GREEN);
    
   switch (state) {
    case active :
      screen->fillRoundRect(location.x, location.y, width, height, 3, activeColor.getColor16());
      break;
    case inactive :
      screen->fillRoundRect(location.x, location.y, width, height, 3, inactiveColor.getColor16());
      break;
    case touched :
      screen->fillRoundRect(location.x+1, location.y+1, width, height, 3, touchedColor.getColor16());
      delay(100);
      break;
    case dragging :
      screen->fillRoundRect(location.x, location.y, width, height, 3, BLUE);
      break;
  };
}



void calcButton::doAction(TS_Point where) {

  mCalc.buttonClick(fStr);
  buttonPressed = true;
}
