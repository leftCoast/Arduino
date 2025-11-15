#include <bmpKeyboard.h>
#include <editable.h>
#include <bitmap.h>
#include <offscreen.h>
#include <resizeBuff.h>

#define KEY_WD  24
#define KEY_HT  33



#define KEYCAP24    "/system/icons/keyboard/keyCap24.bmp"
#define CHECK24     "/system/icons/keyboard/check24.bmp"
#define CHECK36     "/system/icons/keyboard/check36.bmp"
#define CHECK48     "/system/icons/keyboard/check48.bmp"
#define REDX24      "/system/icons/keyboard/x24.bmp"
#define REDX36      "/system/icons/keyboard/x36.bmp"
#define REDX48      "/system/icons/keyboard/x48.bmp"
#define SHIFT24     "/system/icons/keyboard/shift24.bmp"
#define SHIFT36     "/system/icons/keyboard/shift36.bmp"
#define RETURN36    "/system/icons/keyboard/ret36.bmp"
#define RETURN48    "/system/icons/keyboard/ret48.bmp"

#define DELETE24    "/system/icons/keyboard/delete24.bmp"
#define DELETE36    "/system/icons/keyboard/delete36.bmp"
#define L_ARROW24   "/system/icons/keyboard/lArrow24.bmp"
#define R_ARROW24   "/system/icons/keyboard/rArrow24.bmp"
#define SYMBOL24    "/system/icons/keyboard/symb24.bmp"
#define SYMBOL36    "/system/icons/keyboard/symb36.bmp"
#define SPACEB96    "/system/icons/keyboard/spaceB96.bmp"
#define SPACEB72    "/system/icons/keyboard/spaceB72.bmp"


int row(int row) {
  return KEY_HT * (row - 1);
}


// *****************************************************
//                       bmpKeyboard
// *****************************************************


bmpKeyboard::bmpKeyboard(editable* inEditObj,bool modal)
  : keyboard (inEditObj) {

  	rect			sRect;
	offscreen	vPort; 
	bmpObj		keyCap(0,0,KEY_WD,KEY_HT,KEYCAP24);
	
	mKeyTextColor.setColor(LC_YELLOW);					// Set up key text color.
	mKeyTextColor.blend(&white,60);						// Little adjustment here..
	mModal = modal;											// Note if we are modal or not.
	setRect(0, 320 - 4 * KEY_HT, 240, 4 * KEY_HT);	// Set our rectangle.
	if (mKeyBMap.setSize(KEY_WD,KEY_HT)) {				// If we can allocate the RAM for the icon's bitmap..
		vPort.beginDraw(&mKeyBMap);						// Set up to offscreen drawing to the bitmap.
		keyCap.drawSelf(); 									// Draw to it..
		vPort.endDraw();										// Restore normal drawing.
	}
}


bmpKeyboard::~bmpKeyboard(void) { mKeyBMap.clearMap(); }


// Given the colum I'm shooting for and the row I'm in.. 
// What's the colom pixel I start on?
int bmpKeyboard::col(int col, int row) {

  switch (row) {
    case 1 : return KEY_WD * (col - 1);
    case 2 : return KEY_WD * ((col-1) + 0.5);
    case 3 : 
      if (col==1) return 0;
      if (col>1&&col<10) return KEY_WD * ((col-1) + 0.5);
      return KEY_WD * 9;
    case 4 : if (mModal) {
        if (col==9) return KEY_WD * ((col-1) + 0.5);
        return KEY_WD * (col - 1);
      } else {
        if (col>1) return KEY_WD * ((col-1) + 0.5);
        return KEY_WD * (col - 1);
      }
  }
  return 0;	// Shut up compiler!
}


bitmap* bmpKeyboard::getKeyMap(void) { return &mKeyBMap; }

colorObj* bmpKeyboard::getKeyTextColor(void) { return &mKeyTextColor; }


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

  bmpControlKey* shiftKey = new bmpControlKey("^", shift, col(1,3), row(3), KEY_WD*1.5, KEY_HT, this, SHIFT36);
  addObj(shiftKey);
  
  bmpControlKey* backSpKey = new bmpControlKey("<", backspace, col(9,3), row(3), KEY_WD*1.5, KEY_HT, this, DELETE36);
  addObj(backSpKey);
  
  if (mModal) {
    bmpInputKey* spcKey  = new bmpInputKey(" ", " ", " ", col(5,4), row(4), (KEY_WD*3), KEY_HT, this);
    addObj(spcKey);
    bmpControlKey* leftArrow  = new bmpControlKey("<", arrowBack, col(4,4), row(4), KEY_WD, KEY_HT, this, L_ARROW24);
    addObj(leftArrow);
    bmpControlKey* rightArrow  = new bmpControlKey(">", arrowFWD, col(8,4), row(4), KEY_WD, KEY_HT, this, R_ARROW24);
    addObj(rightArrow);
    bmpControlKey* symbolKey   = new bmpControlKey("#", number, col(3,4), row(4), KEY_WD, KEY_HT, this, SYMBOL24);
    addObj(symbolKey);
    bmpControlKey* cancelKey   = new bmpControlKey("x", cancel, col(1,4), row(4), KEY_WD*1.5, KEY_HT, this, REDX36);
    addObj(cancelKey);
    bmpControlKey* oKKey  = new bmpControlKey(">", ok, col(9,4), row(4), KEY_WD*1.5, KEY_HT, this, CHECK36);
    addObj(oKKey);
  } else {
    bmpInputKey* spcKey  = new bmpInputKey(" ", " ", " ", col(4,4), row(4), (KEY_WD*3), KEY_HT, this);
    addObj(spcKey);
    bmpControlKey* leftArrow  = new bmpControlKey("<", arrowBack, col(3,4), row(4), KEY_WD, KEY_HT, this, L_ARROW24);
    addObj(leftArrow);
    bmpControlKey* rightArrow  = new bmpControlKey(">", arrowFWD, col(7,4), row(4), KEY_WD, KEY_HT, this, R_ARROW24);
    addObj(rightArrow);
    bmpControlKey* symbolKey   = new bmpControlKey("#", number, col(1,4), row(4), KEY_WD*1.5, KEY_HT, this, SYMBOL36);
    addObj(symbolKey);
    bmpControlKey* enterKey  = new bmpControlKey(">", enter, col(9,4), row(4), KEY_WD*2, KEY_HT, this, RETURN36);
    addObj(enterKey);
  }
}


// *****************************************************
//                       bmpInputKey
// *****************************************************



bmpInputKey::bmpInputKey(const char* inLabel, const char* inNum, const char* inSym, int inX, int inY, int inWidth, int inHeight, bmpKeyboard* inKeyboard)
: inputKey(inLabel, inNum, inSym, inX, inY, inWidth, inHeight, inKeyboard) {

	mKeyBMap = inKeyboard->getKeyMap();			// The keyboard has the bitmap background.
	setColors(inKeyboard->getKeyTextColor());	// And our favorite text color..
	setTextSize(2);
}

bmpInputKey::~bmpInputKey(void) {  }


void bmpInputKey::drawSelf(void) {
	
	bitmap		ourBMap(mKeyBMap); // Ah! we make a copy of the background.
	rect			sRect;
	offscreen	vPort; 
	colorObj		aPixel;
	point			center;
	point			current;
	float			dist;
	mapper		distMapper(4,12,100,0);
		
	if (buff[0]==' ') {               // Special hack for spacebar.
		if (clicked) {
			screen->fillRect(this, &white);
		} else {
			bmpObj aBmp(x,y,width,height,SPACEB72);
			aBmp.drawSelf();
		}
	} else {                          // "Normal printing
		center.x = width/2;
		center.y = height/2;
		vPort.beginDraw(&ourBMap,x,y);	// We start drawing to it, it already has background image.
		x = x + 7;
		y = y + 9;
		label::drawSelf();
		x = x - 7;
		y = y - 9;
		if (clicked) {
			for(int i=0;i<width;i++) {
				for(int j=0;j<height;j++) {
					aPixel = ourBMap.getColor(i,j);
					current.x = i;
					current.y = j;
					dist = distance(center,current);
					aPixel.blend(&white,distMapper.map(dist));
					ourBMap.setColor(i,j,&aPixel);
				}
			}
		}
		vPort.endDraw();
		screen->blit(x,y,&ourBMap);
	}
}


// *****************************************************
//                       bmpControlKey
// *****************************************************



bmpControlKey::bmpControlKey(const char* inLabel, keyCommands inCom, int inX, int inY, int inWidth, int inHeight, bmpKeyboard* inKeyboard, const char* bmpName)
	: controlKey(inLabel, inCom, inX, inY, inWidth, inHeight, inKeyboard) { ourBMPObj = new bmpObj(inX,inY,inWidth,inHeight,bmpName); }


  bmpControlKey::~bmpControlKey(void) { if (ourBMPObj) delete(ourBMPObj); }


  void bmpControlKey::drawSelf(void) {
    
    if (clicked) {
      screen->fillRect(this, &white);
    } else if (ourBMPObj) {
      	ourBMPObj->setLocation(x,y);	// Just in case we moved. Remember this was just hacked on.
      	ourBMPObj->drawSelf();
    } else {
    	screen->fillRect(this, &red);
    }
  }
