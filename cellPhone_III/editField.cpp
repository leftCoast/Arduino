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

  cursorPos = strlen(buff);
  setTime(500);
  Serial.println(theIdlers.getCount());
  hookup();
  Serial.print("hookedIn:");Serial.println(hookedIn);
  Serial.println(theIdlers.getCount());
  cursorOnOff = false;
}


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
        newBuff[newIndex] = '\0';         // Just in case its the last one
      } else {
        newBuff[newIndex] = buff[i];
      }
      newIndex++;
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

  Serial.print("Cursor position : ");Serial.println(cursorPos);
  Serial.print("Starting str: '");Serial.print(buff);Serial.println("'");
  Serial.print("hookedIn:");Serial.println(hookedIn);
  Serial.println(theIdlers.getCount());
  Serial.print("timer.ding():");Serial.println(ding());
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
  //Serial.print("Edited str  : '");Serial.print(buff);Serial.println("'");
  needRefresh = true;
}


void editField::getCursorPos(int* cursX,int* cursY,int* cursH) {

  *cursH = getTextHeight();
  *cursX = x+(CHAR_WIDTH*textSize*cursorPos);
  *cursY = y;
}


// Need to flash cursor here.       
void editField::idle(void) {

  int cursX;
  int cursY;
  int cursH;

  Serial.println(".");
  if(ding()) {                              // Uggh! Get to work!
    getCursorPos(&cursX,&cursY,&cursH);
    Serial.print("cursX:");Serial.print(cursX);Serial.println();
    Serial.print("cursY:");Serial.print(cursY);Serial.println();
    Serial.print("cursH:");Serial.print(cursH);Serial.println();
    Serial.println();
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
