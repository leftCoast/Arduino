#include "editField.h"



// This fits our rectangle to the text.
// **** TEXT SIZE NEEDS TO BE LOKED INTO *****
editField::editField(char* inText) 
  : label(inText,2) {   

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


}


//bool editField::wantRefresh(void) { return false; } 

       
void editField::idle(void) { }


void editField::drawSelf(void) {

  int hDif;

  colorObj tColor;
  colorObj bColor;

  tColor= ourOS.getColor(SYS_TEDIT_TCOLOR);
  bColor= ourOS.getColor(SYS_TEDIT_BCOLOR);
  setColors(&tColor);
  setJustify(TEXT_LEFT);
  setTextSize(2); // **** TEXT SIZE NEEDS TO BE L)OKED INTO *****
  
  screen->fillRect(x, y, width, height, &bColor);
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
