#include "editField.h"



editField::editField(int inLocX, int inLocY, int inWidth,int inHeight,char* inText,word inTextSize) 
 : label(inLocX,inLocY,inWidth,inHeight,inText) { 

  setTextSize(inTextSize);
  cursorPos = strlen(buff);
  setTime(500);
  hookup();
  cursorOnOff = false;
}


editField::~editField(void) { }


void editField::insertChar(char theChar) {

  int   numChars;
  char* newBuff;
  int   newIndex;
  
  numChars = strlen(buff);     
  newBuff = (char*)malloc(numChars+2);
  if (newBuff) {
    newIndex = 0;
    for (int i=0;i<=numChars;i++) {
      if (i==cursorPos) {
        newBuff[newIndex++] = theChar;
      }
      newBuff[newIndex++] = buff[i];
    }
    cursorPos++;
    free(buff);
    buff = newBuff;
  }
}


void editField::deleteChar(void) {

  int   numChars;
  char* newBuff;
  int   newIndex;

  if (cursorPos>0) {
    cursorPos--;
    numChars = strlen(buff);     
    newBuff = (char*)malloc(numChars);
    if (newBuff) {
      newIndex = 0;
      for (int i=0;i<=numChars;i++) {
        if (i!=cursorPos) {        
          newBuff[newIndex++] = buff[i];
        }
      }
      free(buff);
      buff = newBuff;
    }
  }
}


// Someone pressed a key, deal with it!
// input, backspace, arrowFWD, arrowBack, enter
void editField::handleKeystroke(keystroke* inKeystroke) {

  switch(inKeystroke->editCommand) {
    case input :
      insertChar(inKeystroke->theChar);
    break;
    case backspace :
      deleteChar();
    break;
    case arrowFWD :
      if (cursorPos<strlen(buff)) {
        cursorPos++;
      }
    break;
    case arrowBack :
      if (cursorPos>0) {
        cursorPos--;
      }
    break;
  }
  needRefresh = true;
}


void editField::getCursorPos(int* cursX,int* cursY,int* cursH) {

  *cursH = getTextHeight();
  *cursX = max(x,x+(CHAR_WIDTH*textSize*cursorPos)-1);
  *cursY = y;
}


// Need to flash cursor here.       
void editField::idle(void) {

  int cursX;
  int cursY;
  int cursH;

  if(ding()) {                              // Uggh! Get to work!
    getCursorPos(&cursX,&cursY,&cursH);
    if(cursorOnOff) {
      screen->drawVLine(cursX,cursY,cursH,&white);
    } else {
      screen->drawVLine(cursX,cursY,cursH,&black);
    }
    cursorOnOff = !cursorOnOff;
    stepTime();
  }
}


void editField::drawSelf(void) {

  int hDif;

  colorObj tColor;
  colorObj bColor;

  tColor= ourOS.getColor(SYS_TEDIT_TCOLOR);
  bColor= ourOS.getColor(SYS_TEDIT_BCOLOR);
  setColors(&tColor);
  setJustify(TEXT_LEFT);
  
  screen->fillRect(x, y, width, height, &bColor);
  //screen->drawRect(x, y, width, height, &red);
  label::drawSelf();
}
