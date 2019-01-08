#include "controlPanel.h"
#include "songList.h"

extern songList*   ourList;
extern songList*   ourController;
extern colorObj    screenBColor;
extern colorObj    textUHColor;
extern colorObj    textHColor;



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


// ************************************
// ************ toggleBtn *************
// ************************************


toggleBtn::toggleBtn(int x, int y, word width,word height)
  :scrollingList(x,y,width,height,dialScroll) {

  toggleItem* newItem;
  
  newItem = new toggleItem(this,toggleDn);
  addObj(newItem);

  newItem = new toggleItem(this,toggleLft);
  addObj(newItem);
}


toggleBtn::~toggleBtn(void) {  }


void toggleBtn::doPotVal(int aVal) {

  mapper  potToButton(0,1024,0,100);
  float   pcnt;
  
  pcnt = potToButton.Map(aVal);
  setScrollValue(pcnt);
}


void toggleBtn::drawSelf(void) { }



// ************************************
// *********** controlPanel ***********
// ************************************


controlPanel::controlPanel(int x, int y, word width,word height)
  : scrollingList(x,y,width,height,dSOpenBtm),
  pushPotGUI() {

  potSmoother = new runningAvg(10);
  potToControl = new mapper(0,1024,0,100);
}


controlPanel::~controlPanel(void) {

  if (potSmoother) delete potSmoother;
  if (potToControl) delete potToControl;
 }


void controlPanel::drawSelf(void) {
  screen->fillRect(x,y,width,height,&red);
}


void controlPanel::reset(void) { needRefresh = true; }


void controlPanel::doPotVal(int aVal) {

  float pcnt;
  
  pcnt = potToControl->Map(aVal);
  setScrollValue(pcnt);
}
