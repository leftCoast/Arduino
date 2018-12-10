#include "editField.h"



editField::editField(int inLocX, int inLocY, int inWidth,int inHeight,char* inText,word inTextSize) 
 : label(inLocX,inLocY,inWidth,inHeight,inText,inTextSize) { 

  cursorPos = strlen(buff);
  setTime(500);
  hookup();
  cursorOnOff = false;
  setColors(&black,&white);   // Good default.
}


editField::~editField(void) { }


// When an outside force sets any value, it passes through here.
// We need to deal with the cursor. Be careful not to call our setValue()
// in here by accadent. It'll probably mess up your poor cursor.
void editField::setValue(char* str) {

  label::setValue(str);
  cursorPos = strlen(buff);
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
