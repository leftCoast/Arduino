#include "bmpKeyboard.h"

#include <editField.h>

#define KEY_WD  24
#define KEY_HT  33



#define KEYCAP24    "/icons/keyCap24.bmp"
#define CHECK24     "/icons/check24.bmp"
#define REDX24      "/icons/x24.bmp"
#define SHIFT24     "/icons/shift24.bmp"
#define RETURN48    "/icons/ret48.bmp"

#define DELETE24    "/icons/delete24.bmp"
#define L_ARROW24   "/icons/lArrow24.bmp"
#define R_ARROW24   "/icons/rArrow24.bmp"
#define SYMBOL24    "/icons/symb24.bmp"
#define SPACEB96    "/icons/spaceB96.bmp"



int col(int col, int row) {

  switch (row) {
    case 1 : return KEY_WD * (col - 1);
    case 2 : return KEY_WD * ((col-1) + 0.5);
    case 3 : 
      if (col==1) return 0;
      if (col>1&&col<10) return KEY_WD * ((col-1) + 0.5);
      return KEY_WD * 9;
    case 4 : return KEY_WD * (col - 1);
  }
}


int row(int row) {
  return KEY_HT * (row - 1);
}

bmpPipe keyCap; // No point in having lots and lots of the same thing.

// *****************************************************
//                       bmpKeyboard
// *****************************************************


bmpKeyboard::bmpKeyboard(editField* inEditField,bool modal)
  : keyboard (inEditField) {

  rect  sRect;

  mModal = modal;
  setRect(0, 320 - 4 * KEY_HT, 240, 4 * KEY_HT);
  keyCap.openPipe(KEYCAP24);
  sRect.setRect(0, 0, KEY_WD, KEY_HT);
  keyCap.setSourceRect(sRect);
}

bmpKeyboard::~bmpKeyboard(void) {  }



void bmpKeyboard::loadKeys(void) {

  bmpInputKey* qKey = new bmpInputKey("q", "1", "", col(1,1), row(1), KEY_WD, KEY_HT, this);
  bmpInputKey* wKey = new bmpInputKey("w", "2", "", col(2,1), row(1), KEY_WD, KEY_HT, this);
  bmpInputKey* eKey = new bmpInputKey("e", "3", "", col(3,1), row(1), KEY_WD, KEY_HT, this);
  bmpInputKey* rKey = new bmpInputKey("r", "4", "", col(4,1), row(1), KEY_WD, KEY_HT, this);
  bmpInputKey* tKey = new bmpInputKey("t", "5", "", col(5,1), row(1), KEY_WD, KEY_HT, this);
  bmpInputKey* yKey = new bmpInputKey("y", "6", "", col(6,1), row(1), KEY_WD, KEY_HT, this);
  bmpInputKey* uKey = new bmpInputKey("u", "7", "", col(7,1), row(1), KEY_WD, KEY_HT, this);
  bmpInputKey* iKey = new bmpInputKey("i", "8", "", col(8,1), row(1), KEY_WD, KEY_HT, this);
  bmpInputKey* oKey = new bmpInputKey("o", "9", "", col(9,1), row(1), KEY_WD, KEY_HT, this);
  bmpInputKey* pKey = new bmpInputKey("p", "0", "", col(10,1), row(1), KEY_WD, KEY_HT, this);

  addObj(qKey);
  addObj(wKey);
  addObj(eKey);
  addObj(rKey);
  addObj(tKey);
  addObj(yKey);
  addObj(uKey);
  addObj(iKey);
  addObj(oKey);
  addObj(pKey);

  bmpInputKey* aKey = new bmpInputKey("a", "-", "", col(1,2), row(2), KEY_WD, KEY_HT, this);
  bmpInputKey* sKey = new bmpInputKey("s", "/", "", col(2,2), row(2), KEY_WD, KEY_HT, this);
  bmpInputKey* dKey = new bmpInputKey("d", ":", "", col(3,2), row(2), KEY_WD, KEY_HT, this);
  bmpInputKey* fKey = new bmpInputKey("f", ";", "", col(4,2), row(2), KEY_WD, KEY_HT, this);
  bmpInputKey* gKey = new bmpInputKey("g", "(", "", col(5,2), row(2), KEY_WD, KEY_HT, this);
  bmpInputKey* hKey = new bmpInputKey("h", ")", "", col(6,2), row(2), KEY_WD, KEY_HT, this);
  bmpInputKey* jKey = new bmpInputKey("j", "$", "", col(7,2), row(2), KEY_WD, KEY_HT, this);
  bmpInputKey* kKey = new bmpInputKey("k", "@", "", col(8,2), row(2), KEY_WD, KEY_HT, this);
  bmpInputKey* lKey = new bmpInputKey("l", "\"", "", col(9,2), row(2), KEY_WD, KEY_HT, this);

  addObj(aKey);
  addObj(sKey);
  addObj(dKey);
  addObj(fKey);
  addObj(gKey);
  addObj(hKey);
  addObj(jKey);
  addObj(kKey);
  addObj(lKey);

  bmpInputKey* zKey = new bmpInputKey("z", ".", "", col(2,3), row(3), KEY_WD, KEY_HT, this);
  bmpInputKey* xKey = new bmpInputKey("x", ",", "", col(3,3), row(3), KEY_WD, KEY_HT, this);
  bmpInputKey* cKey = new bmpInputKey("c", "?", "", col(4,3), row(3), KEY_WD, KEY_HT, this);
  bmpInputKey* vKey = new bmpInputKey("v", "!", "", col(5,3), row(3), KEY_WD, KEY_HT, this);
  bmpInputKey* bKey = new bmpInputKey("b", "&", "", col(6,3), row(3), KEY_WD, KEY_HT, this);
  bmpInputKey* nKey = new bmpInputKey("n", "%", "", col(7,3), row(3), KEY_WD, KEY_HT, this);
  bmpInputKey* mKey = new bmpInputKey("m", "#", "", col(8,3), row(3), KEY_WD, KEY_HT, this);

  addObj(zKey);
  addObj(xKey);
  addObj(cKey);
  addObj(vKey);
  addObj(bKey);
  addObj(nKey);
  addObj(mKey);

  bmpInputKey* spcKey  = new bmpInputKey(" ", " ", " ", col(4,1), row(4), (KEY_WD*4), KEY_HT, this);
  addObj(spcKey);

  bmpControlKey* shiftKey = new bmpControlKey("^", shift, col(1,3), row(3), KEY_WD, KEY_HT, this, SHIFT24);
  addObj(shiftKey);
  
  bmpControlKey* backSpKey = new bmpControlKey("<", backspace, col(10,3), row(3), KEY_WD, KEY_HT, this, DELETE24);
  addObj(backSpKey);
  
  bmpControlKey* leftArrow  = new bmpControlKey("<", arrowBack, col(3,4), row(4), KEY_WD, KEY_HT, this, L_ARROW24);
  addObj(leftArrow);
  
  bmpControlKey* rightArrow  = new bmpControlKey(">", arrowFWD, col(8,4), row(4), KEY_WD, KEY_HT, this, R_ARROW24);
  addObj(rightArrow);
  
  bmpControlKey* symbolKey   = new bmpControlKey("#", number, col(1,4), row(4), KEY_WD, KEY_HT, this, SYMBOL24);
  addObj(symbolKey);
  
  if (mModal) {
    bmpControlKey* cancelKey   = new bmpControlKey("x", cancel, col(9,4), row(4), KEY_WD, KEY_HT, this, REDX24);
    addObj(cancelKey);
    bmpControlKey* oKKey  = new bmpControlKey(">", ok, col(10,4), row(4), KEY_WD, KEY_HT, this, CHECK24);
    addObj(oKKey);
  } else {
    bmpControlKey* enterKey  = new bmpControlKey(">", enter, col(9,4), row(4), KEY_WD*2, KEY_HT, this, RETURN48);
    addObj(enterKey);
  }
}



// *****************************************************
//                       bmpInputKey
// *****************************************************



bmpInputKey::bmpInputKey(char* inLabel, char* inNum, char* inSym, int inX, int inY, int inWidth, int inHeight, bmpKeyboard* inKeyboard)
: inputKey(inLabel, inNum, inSym, inX, inY, inWidth, inHeight, inKeyboard) {

  setColors(&white);
  setTextSize(2);
}

  bmpInputKey::~bmpInputKey(void) {  }


  void bmpInputKey::drawSelf(void) {

    if (clicked) {
      screen->fillRect(this, &white);
    } else {
      if (buff[0]==' ') {               // Special hack for spacebar.
        rect  sRect(0,0,width,height);
        bmpPipe aPipe(sRect);
        aPipe.openPipe(SPACEB96);
        aPipe.drawBitmap(x,y);
      } else {                          // "Normal printing
        keyCap.drawBitmap(x, y);
        x = x + 7;
        y = y + 9;
        label::drawSelf();
        x = x - 7;
        y = y - 9;
      }
    }
  }



  // *****************************************************
  //                       bmpControlKey
  // *****************************************************




  bmpControlKey::bmpControlKey(char* inLabel, keyCommands inCom, int inX, int inY, int inWidth, int inHeight, bmpKeyboard * inKeyboard, char* bmpPath)
    : controlKey(inLabel, inCom, inX, inY, inWidth, inHeight, inKeyboard) {

    rect  sRect;

    sRect.setRect(0, 0, width, height);
    mBmpPipe.setSourceRect(sRect);
    mBmpPipe.openPipe(bmpPath);
  }


  bmpControlKey::~bmpControlKey(void) {  }


  void bmpControlKey::drawSelf(void) {
    
    if (clicked) {
      screen->fillRect(this, &white);
    } else {
      mBmpPipe.drawBitmap(x, y);
    }
  }
