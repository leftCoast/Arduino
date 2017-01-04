 #include "displayObj.h"
  
  displayObj::displayObj(boolean inGraphic,boolean inColor,boolean inTouch,boolean inSD) {

    graphic = inGraphic;
    color = inColor;
    hasTouch = inTouch;
    hasSD = inSD;
  }

void displayObj::begin(void) { }
void displayObj::setTextColor(colorObj* inColor) { }
void displayObj::setTextSize(byte inSize) { }
void displayObj::setCursor(int inX,int inY) { }
void displayObj::drawText(char* inText) { }
void displayObj::fillScreen(colorObj* inColor) { }

bool displayObj::isGraphic(void) { return graphic; }
bool displayObj::isColor(void) { return color; }
bool displayObj::hasTouchScreen(void) { return hasTouch; }
bool displayObj::hadSDDrive(void) { return hasSD; }
  

