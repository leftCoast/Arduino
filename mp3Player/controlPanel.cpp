#include "controlPanel.h"
#include "songList.h"

extern songList*    ourList;
extern songList*    ourController;
extern bool         randomPlay;
extern colorObj     screenBColor;
extern colorObj     textUHColor;
extern colorObj     textHColor;
extern colorObj     textActiveColor;



// ************************************
// ************ contPnlItem ***********
// ************************************


contPnlItem::contPnlItem(drawList* myList,CPIType myType)
  : drawObj(0,0,myList->width-1,myList->height-1) {

  mDrawList = myList;
  mCPIType = myType;
}


contPnlItem::~contPnlItem(void) {  }


void contPnlItem::doAction(void) {

  if (mCPIType==randPlayOn) {
    randomPlay = true;
  } else {
    randomPlay = false;
  }
}


void contPnlItem::drawSelf(void) {

  if (mDrawList->isVisible(this)) {
    screen->setTextSize(1);
    screen->setTextWrap(false);
    screen->setCursor(1,1);
    screen->fillRect(x,y,width,height,&screenBColor);
    if (focus) {
      screen->setTextColor(&textHColor);
    } else if (randomPlay) {
      screen->setTextColor(&textActiveColor);
    } else {
      screen->setTextColor(&textUHColor);
    }
   if (mCPIType==randPlayOn) {
      screen->drawText("Random play on");
    } else {
      screen->drawText("Random play off");
    }
  }
}



// ************************************
// *********** controlPanel ***********
// ************************************


controlPanel::controlPanel(int x, int y, word width,word height)
  : scrollingList(x,y,width,height,dSOpenBtm),
  pushPotGUI() {

  potSmoother = new runningAvg(20);
  potToControl = new mapper(0,1024,0,100);

  mOnItem = new contPnlItem(this,randPlayOn);
  addObj(mOnItem);
  mOffItem = new contPnlItem(this,randPlayOff);
  addObj(mOffItem);
}


controlPanel::~controlPanel(void) {

  if (potSmoother) delete potSmoother;
  if (potToControl) delete potToControl;
 }


void controlPanel::drawSelf(void) {  }


void controlPanel::reset(void) { }


void controlPanel::offList(void) {

  setFocusPtr(NULL);
  setControlPtr(ourList);
  if (randomPlay) {
    showItem(mOnItem);
  } else {
    showItem(mOffItem);
  }
}


void controlPanel::doPotVal(int aVal) {

  float pcnt;
  
  pcnt = potToControl->Map(aVal);
  setScrollValue(pcnt);
}
