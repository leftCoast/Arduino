#include "keyboard.h"
#include  "litlOS.h"


#define COL_2 COL_1 + KEY_WD + COL_SP
#define COL_3 COL_2 + KEY_WD + COL_SP
#define COL_4 COL_3 + KEY_WD + COL_SP
#define COL_5 COL_4 + KEY_WD + COL_SP
#define COL_6 COL_5 + KEY_WD + COL_SP
#define COL_7 COL_6 + KEY_WD + COL_SP
#define COL_8 COL_7 + KEY_WD + COL_SP
#define COL_9 COL_8 + KEY_WD + COL_SP
#define COL_10 COL_9 + KEY_WD + COL_SP

#define COL_21 12
#define COL_22 COL_21 + KEY_WD + COL_SP
#define COL_23 COL_22 + KEY_WD + COL_SP
#define COL_24 COL_23 + KEY_WD + COL_SP
#define COL_25 COL_24 + KEY_WD + COL_SP
#define COL_26 COL_25 + KEY_WD + COL_SP
#define COL_27 COL_26 + KEY_WD + COL_SP
#define COL_28 COL_27 + KEY_WD + COL_SP
#define COL_29 COL_28 + KEY_WD + COL_SP


#define ROW_2 ROW_1 + KEY_HT + ROW_SP
#define ROW_3 ROW_2 + KEY_HT + ROW_SP
#define ROW_4 ROW_3 + KEY_HT + ROW_SP



// ********************************************************************************
// ********************************************************************************
// keyboardKeys are the base class of all keys. This is so there is a common interface
// to the keyboard object.
// ********************************************************************************
// ********************************************************************************


keyboardKey::keyboardKey(keyboard* inKeyboard) {

  mKeyboard = inKeyboard;
  mState = mKeyboard->getState();
  mCom = input;
  hookup();
}


keyboardKey::~keyboardKey(void) { }



// ********************************************************************************
// ********************************************************************************
// inputKeys are for inputting data. Like letters & numbers.
// ********************************************************************************
// ********************************************************************************


inputKey::inputKey(char* inLabel,char* inNum,char* inSym,word locX, word locY,byte width,byte height,keyboard* inKeyboard)
  : keyboardKey(inKeyboard),
  label(locX,locY,width-1,height-1,inLabel) {
    
    clickable(true);
    mChar = buff[0];
    mNum = inNum[0];
    mSymbol = inSym[0];
  }
  

  
inputKey::~inputKey(void) { }


void inputKey::drawSelf(void) {

  colorObj baseColor = ourOS.getColor(SYS_INPUT_BTN_COLOR);
  colorObj textColor = ourOS.getColor(SYS_CONTROL_LBL_COLOR);
  colorObj shadowColor = ourOS.getColor(SYS_SHADOW_COLOR);
                                                  
  screen->drawRect(x+1, y+1, width-1, height-1, &shadowColor);
  if (clicked) {
    screen->fillRect(x, y, width-1, height-1, &black);
    setColors(&white);
  } else {
    screen->fillRect(x, y, width-1, height-1, &baseColor);
    setColors(&textColor);
  }
  
  setTextSize(2);
  setJustify(TEXT_CENTER);
  x = x+5;
  y = y+3;
  label::drawSelf();
  x = x-5;
  y = y-3;
}

  
void inputKey::doAction(void)  {
  
  mKeyboard->handleKey(buff[0]);      // Toss out our input.
  if (mKeyboard->mState==shifted) {   // If we're in a shift state..
      mKeyboard->handleKey(chars);    // Clear it.
  }
}


// chars, shifted, numbers, symbols
void inputKey::idle() {

  if (mKeyboard->mState!=mState) {
    switch(mKeyboard->mState) {
      case chars :
        buff[0] = tolower(mChar);
      break;
      case shifted :
        buff[0] = toupper(mChar);
      break;
      case numbers :
        buff[0] = mNum;
      break;
      case symbols :
        buff[0] = mSymbol;
      break;
    }
    mState = mKeyboard->mState;
    needRefresh = true;
  }
}


// ********************************************************************************
// ********************************************************************************
// controlKeys are for doing everything else. Like shift, delete and arrow keys.
// ********************************************************************************
// ********************************************************************************


controlKey::controlKey(char* inLabel,keyCommands inCom,word locX, word locY,byte width,byte height,keyboard* inKeyboard)
  : keyboardKey(inKeyboard),
  label(locX,locY,width-1,height-1,inLabel) {

  clickable(true);
  mCom = inCom;
  if(inCom==enter){
    buff[0] = (char)175;
  }
}


controlKey::~controlKey(void) { }


void controlKey::drawSelf(void) {
    
  colorObj baseColor = ourOS.getColor(SYS_CONTROL_BTN_COLOR);
  colorObj textColor = ourOS.getColor(SYS_CONTROL_LBL_COLOR);
  if (mCom==backspace) {
    textColor.setColor(LC_RED);
  }
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
  x = x+5;
  y = y+3;
  label::drawSelf();
  x = x-5;
  y = y-3;
}


void controlKey::handleShift() {
  
  switch(mKeyboard->mState) {
    case chars    : mKeyboard->handleKey(shifted); break;
    case shifted  : mKeyboard->handleKey(chars); break;
    case number   : mKeyboard->handleKey(shifted); break;
    default       : break;  // Shut up, compiler.
  }
}


void controlKey::handleNumber() {
  
  switch(mKeyboard->mState) {
    case chars    :
    case shifted  : mKeyboard->handleKey(numbers); break;
    case number   : mKeyboard->handleKey(chars); break;
    default       : break;  // Shut up, compiler.
  }
}

      
// enum  keyCommands { input, shift, number, symbol, backspace, arrowFWD, arrowBack, enter };
void controlKey::doAction(void) {

  switch (mCom) {
    case backspace  :  
    case arrowFWD   :
    case arrowBack  : 
    case enter      : mKeyboard->handleKey(mCom); break;
    case shift      : handleShift(); break;
    case number     : handleNumber(); break;
    default         : break;  // Shut up, compiler. 
  }
}



// ********************************************************************************
// ********************************************************************************
// keyboard, One object to control them all one object to bind them..
// Well, its kida' like that for the keys.
// ********************************************************************************
// ********************************************************************************


keyboard::keyboard(editField* inEditField) {

  mEditField = inEditField;
  mState = chars;
          
  qKey = new inputKey("q","1","",COL_1,ROW_1,KEY_WD,KEY_HT,this);
  wKey = new inputKey("w","2","",COL_2,ROW_1,KEY_WD,KEY_HT,this);
  eKey = new inputKey("e","3","",COL_3,ROW_1,KEY_WD,KEY_HT,this);
  rKey = new inputKey("r","4","",COL_4,ROW_1,KEY_WD,KEY_HT,this);
  tKey = new inputKey("t","5","",COL_5,ROW_1,KEY_WD,KEY_HT,this);
  yKey = new inputKey("y","6","",COL_6,ROW_1,KEY_WD,KEY_HT,this);
  uKey = new inputKey("u","7","",COL_7,ROW_1,KEY_WD,KEY_HT,this);
  iKey = new inputKey("i","8","",COL_8,ROW_1,KEY_WD,KEY_HT,this);
  oKey = new inputKey("o","9","",COL_9,ROW_1,KEY_WD,KEY_HT,this);
  pKey = new inputKey("p","0","",COL_10,ROW_1,KEY_WD,KEY_HT,this);
  
  aKey = new inputKey("a","-","",COL_21,ROW_2,KEY_WD,KEY_HT,this);
  sKey = new inputKey("s","/","",COL_22,ROW_2,KEY_WD,KEY_HT,this);
  dKey = new inputKey("d",":","",COL_23,ROW_2,KEY_WD,KEY_HT,this);
  fKey = new inputKey("f",";","",COL_24,ROW_2,KEY_WD,KEY_HT,this);
  gKey = new inputKey("g","(","",COL_25,ROW_2,KEY_WD,KEY_HT,this);
  hKey = new inputKey("h",")","",COL_26,ROW_2,KEY_WD,KEY_HT,this);
  jKey = new inputKey("j","$","",COL_27,ROW_2,KEY_WD,KEY_HT,this);
  kKey = new inputKey("k","@","",COL_28,ROW_2,KEY_WD,KEY_HT,this);
  lKey = new inputKey("l","\"","",COL_29,ROW_2,KEY_WD,KEY_HT,this);
  
  zKey = new inputKey("z",".","",COL_22,ROW_3,KEY_WD,KEY_HT,this);
  xKey = new inputKey("x",",","",COL_23,ROW_3,KEY_WD,KEY_HT,this);
  cKey = new inputKey("c","?","",COL_24,ROW_3,KEY_WD,KEY_HT,this);
  vKey = new inputKey("v","!","",COL_25,ROW_3,KEY_WD,KEY_HT,this);
  bKey = new inputKey("b","&","",COL_26,ROW_3,KEY_WD,KEY_HT,this);
  nKey = new inputKey("n","%","",COL_27,ROW_3,KEY_WD,KEY_HT,this);
  mKey = new inputKey("m","#","",COL_28,ROW_3,KEY_WD,KEY_HT,this);
  
  spcKey  = new inputKey(" "," "," ",COL_24+1,ROW_4,(KEY_WD*3),KEY_HT,this);

  //input, shift, symbol, backspace, arrowFWD, arrowBack, enter 
  
  shiftKey = new controlKey("^",shift,COL_1,ROW_3,KEY_WD+COL_21-COL_1-(2*ROW_SP),KEY_HT,this);
  backSpKey = new controlKey("<",backspace,COL_29+(2*ROW_SP),ROW_3,KEY_WD+COL_21-COL_1-(2*ROW_SP),KEY_HT,this);
  leftArrow  = new controlKey("<",arrowBack,COL_23,ROW_4,KEY_WD,KEY_HT,this);
  rightArrow  = new controlKey(">",arrowFWD,COL_27,ROW_4,KEY_WD,KEY_HT,this);
  symbolKey   = new controlKey("#",number,COL_1,ROW_4,KEY_WD+COL_21-COL_1-(2*ROW_SP),KEY_HT,this);
  enterKey  = new controlKey(">",enter,COL_29+(2*ROW_SP),ROW_4,KEY_WD+COL_21-COL_1-(2*ROW_SP),KEY_HT,this);
          
  viewList.addObj(qKey);
  viewList.addObj(wKey);
  viewList.addObj(eKey);
  viewList.addObj(rKey);
  viewList.addObj(tKey);
  viewList.addObj(yKey);
  viewList.addObj(uKey);
  viewList.addObj(iKey);
  viewList.addObj(oKey);
  viewList.addObj(pKey);

  viewList.addObj(aKey);
  viewList.addObj(sKey);
  viewList.addObj(dKey);
  viewList.addObj(fKey);
  viewList.addObj(gKey);
  viewList.addObj(hKey);
  viewList.addObj(jKey);
  viewList.addObj(kKey);
  viewList.addObj(lKey);

  viewList.addObj(zKey);
  viewList.addObj(xKey);
  viewList.addObj(cKey);
  viewList.addObj(vKey);
  viewList.addObj(bKey);
  viewList.addObj(nKey);
  viewList.addObj(mKey);
  
  viewList.addObj(spcKey);
  
  viewList.addObj(shiftKey);
  viewList.addObj(backSpKey);
  viewList.addObj(leftArrow);
  viewList.addObj(rightArrow);
  viewList.addObj(symbolKey);
  viewList.addObj(enterKey);
}


keyboard::~keyboard(void) {

  if (qKey) delete qKey;
  if (wKey) delete wKey;
  if (eKey) delete eKey;
  if (rKey) delete rKey;
  if (tKey) delete tKey;
  if (yKey) delete yKey;
  if (uKey) delete uKey;
  if (iKey) delete iKey;
  if (oKey) delete oKey;
  if (pKey) delete pKey;
   
  if (aKey) delete aKey;
  if (sKey) delete sKey;
  if (dKey) delete dKey;
  if (fKey) delete fKey;
  if (gKey) delete gKey;
  if (hKey) delete hKey;
  if (jKey) delete jKey;
  if (kKey) delete kKey;
  if (lKey) delete lKey;
  
  if (zKey) delete zKey;
  if (xKey) delete xKey;
  if (cKey) delete cKey;
  if (vKey) delete vKey;
  if (bKey) delete bKey;
  if (nKey) delete nKey;
  if (mKey) delete mKey;
  
  if (spcKey) delete spcKey;
  
  if (shiftKey) delete shiftKey;
  if (backSpKey) delete backSpKey;
  if (leftArrow) delete leftArrow;
  if (rightArrow) delete rightArrow;
  if (symbolKey) delete symbolKey;
  if (enterKey) delete enterKey;
}


void keyboard::handleKey(char inChar) {

  keystroke aKeystroke;

  aKeystroke.editCommand = input;
  aKeystroke.theChar = inChar;
  mEditField->handleKeystroke(&aKeystroke);
  if(mState==shifted) {
    mState = chars;
  }
}


void keyboard::handleKey(keyCommands inEditCom) { 

  keystroke aKeystroke;

  aKeystroke.editCommand = inEditCom;
  aKeystroke.theChar = '\0';
  mEditField->handleKeystroke(&aKeystroke);
}


void keyboard::handleKey(keyStates inState) { mState = inState; }

keyStates keyboard::getState(void) { return mState; }
