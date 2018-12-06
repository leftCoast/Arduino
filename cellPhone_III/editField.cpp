#include "editField.h"



// This fits our rectangle to the text.
editField::editField(char* inText) 
  : label(inText,DEF_TEDIT_TSIZE) {

  initEditField();
  width = width+2;
  height = height+2;
}


editField::editField(int inLocX, int inLocY, int inWidth,int inHeight,char* inText) 
 : label(inLocX,inLocY,inWidth,inHeight,inText) { initEditField(); }


editField::editField(int inLocX, int inLocY, int inWidth,int inHeight)
  : label(inLocX,inLocY,inWidth,inHeight) { initEditField(); }


editField::~editField(void) { }


void editField::initEditField() {

  colorObj tColor;
  colorObj bColor;

  tColor.setColor(DEF_TEDIT_TCOLOR);
  bColor.setColor(DEF_TEDIT_BCOLOR);
  setColors(&tColor,&bColor);
  setJustify(TEXT_LEFT);
  setTextSize(DEF_TEDIT_TSIZE);
}


//bool editField::wantRefresh(void) { return false; } 

       
void editField::idle(void) { }


void editField::drawSelf(void) {

  int hDif;
  
  screen->fillRect(x, y, width, height, &backColor);
  hDif = height-getTextHeight();
  if (hDif>=2) {
    hDif = hDif/2;
    x = x + hDif;
    y = y + hDif;
    label::drawSelf();
    x = x - hDif;
    y = y - hDif;
  } else {
    label::drawSelf();
  }
}
