#include "controlPanel.h"
#include "songList.h"

extern songList*    ourList;
extern songList*    ourController;
extern toggleBtn*   ourToggler;
extern bool         randomPlay;
extern colorObj     screenBColor;
extern colorObj     textUHColor;
extern colorObj     textHColor;
extern colorObj     textActiveColor;


// ************************************
// ************* toggleItem ***********
// ************************************


toggleItem::toggleItem(drawList* myList,toggleDir dir)
  : drawObj(1,1,10,10) {

  mDir = dir;
  mList = myList;
  needRefresh = false;
}


toggleItem::~toggleItem(void) {  }

void toggleItem::doAction(void) {

  if (mDir==toggleLft) {
    //setControlPtr(ourController); // After we get one..
    setControlPtr(ourList);         // Knock this out.
  } else {
    setControlPtr(ourList);
  }
}


void toggleItem::drawSelf(void) {

  if (mList->isVisible(this)) {
  screen->fillRect(x,y,width,height,&screenBColor);
    if (focus) {
      screen->drawRect(x,y,width,height,&textHColor);
      if (mDir==toggleLft) {
        screen->drawHLine(0,5,10,&textHColor);
      } else {
        screen->drawVLine(5,0,10,&textHColor);
      }
    } else {
      screen->drawRect(x,y,width,height,&textUHColor);
      if (mDir==toggleDn) {
        screen->drawHLine(x,y+5,10,&textUHColor);
      } else {
        screen->drawVLine(x+5,y,10,&textUHColor);
      }
    }
  }
}


// ************************************
// ************ toggleBtn *************
// ************************************


toggleBtn::toggleBtn(int x, int y, word width,word height)
  :scrollingList(x,y,width,height,dSOpenBoth) {

  toggleItem* newItem;
  
  newItem = new toggleItem(this,toggleDn);
  addObj(newItem);

  newItem = new toggleItem(this,toggleLft);
  addObj(newItem);
}


toggleBtn::~toggleBtn(void) {  }


void toggleBtn::doPotVal(int aVal) {
  
  int amp=500;
  
  mapper  potToButton(0+amp,1024-amp,0,100);
  float   pcnt;
  
  pcnt = potToButton.Map(aVal);
  setScrollValue(pcnt);
}


void toggleBtn::offList(void) {

  if (mLoc == aboveList) {
    setControlPtr(ourController);
  } else if (mLoc == belowList) {
    setControlPtr(ourList);
  }
}
  
void toggleBtn::drawSelf(void) { }



// ************************************
// ************ contPnlItem ***********
// ************************************


contPnlItem::contPnlItem(drawList* myList,CPIType myType)
  : drawObj(0,0,myList->width-1,myList->height-1) {

  Serial.println("In contPnlItem Building contPnlItem, Woo Hoo!");
  Serial.flush();

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
      screen->setTextColor(&red);
    } else {
      screen->setTextColor(&textHColor);
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

  contPnlItem*  newItem; 
   
  potSmoother = new runningAvg(10);
  potToControl = new mapper(0,1024,0,100);
  
  Serial.println("Building contPnlItem");
  Serial.flush();
  newItem = new contPnlItem(this,randPlayOn);
  Serial.println("Adding contPnlItem to ourselves.");
  Serial.flush();
  addObj(newItem);
  Serial.println("Building second contPnlItem");
  Serial.flush();
  newItem = new contPnlItem(this,randPlayOff);
  Serial.println("Adding second contPnlItem to ourselves.");
  Serial.flush();
  addObj(newItem);
}


controlPanel::~controlPanel(void) {

  if (potSmoother) delete potSmoother;
  if (potToControl) delete potToControl;
 }


void controlPanel::drawSelf(void) {  }


void controlPanel::reset(void) { }


void controlPanel::offList(void) { setControlPtr(ourToggler); }


void controlPanel::doPotVal(int aVal) {

  float pcnt;
  
  pcnt = potToControl->Map(aVal);
  setScrollValue(pcnt);
}
