#include <editField.h>


editField::editField(rect* inRect,char* inText,word inTextSize)
	: label(inRect,inText,inTextSize) {
	
	cursorPos = strlen(buff);
	setTime(500);
	hookup();
	cursorOnOff = false;
	returnExit  = true;
	setColors(&black,&white);   // Good default.
	parent = NULL;
}

	
editField::editField(int inLocX, int inLocY, int inWidth,int inHeight,char* inText,word inTextSize) 
 : label(inLocX,inLocY,inWidth,inHeight,inText,inTextSize) { 

  cursorPos = strlen(buff);
  setTime(500);
  hookup();
  cursorOnOff = false;
  returnExit  = true;
  setColors(&black,&white);   // Good default.
  parent = NULL;
}


editField::~editField(void) { }


// Someone using us as an editing field and want to know when we loose focus?
// This is where they tell us who they are so we can inform them when focus is lost.
void editField::setParent(drawObj*	inParent) { parent = inParent; }


// Many times people expect hitting the return key should exit the edit field,
// settin this to true sets this behaviour.
void editField::setRetExit(bool inRetExit) { returnExit = inRetExit; }
				
				
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
		case enter		:
			if (returnExit) {
				setFocus(NULL);
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

	if (focus) {
		if(ding()) {                              // Uggh! Get to work!
			cursorOnOff = !cursorOnOff;
			needRefresh = true;
			start();
		}
	}
}


void editField::drawSelf(void) {

	int cursX;
	int cursY;
	int cursH;
	
	label::drawSelf();
	if (focus) {
		getCursorPos(&cursX,&cursY,&cursH);
		if(cursorOnOff) {
			screen->drawVLine(cursX,cursY,cursH,&textColor);
		} else {
			screen->drawVLine(cursX,cursY,cursH,&backColor);
		}
	}
}
 

// Its not uncommon for a drawObject to want to incorporate an edit field into itself.
// The issue is that the parent object will get its focus set, then pass it on to us to
// edit stuff. But never gets the message that focus has been lost. This patches that
// issue by allowing this to pass back its unfocus call to its parent.
void editField::setFocus(bool setLoose) {
	
	label::setFocus(setLoose);
	if (!setLoose && parent) {
		parent->setFocus(false);
	}
}
    