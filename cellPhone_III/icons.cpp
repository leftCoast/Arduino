#include "icons.h"
#include "litlOS.h"

barGraphBar::barGraphBar(int inX,int inY,int inwidth,int inHeight,barGraphDir inDir,bool inTouch)
  : drawObj(inX,inY,inwidth,inHeight,inTouch) {

  mDir = inDir;
  mBarVal = 0;
  needRefresh = false;  // Why?! Because we are not ready to be drawn yet.
}


barGraphBar::~barGraphBar(void) {  }


void barGraphBar::setLimits(float inStart,float inEnd,bool leaveAPixle) {

  int i;

  if (leaveAPixle) {    // Sometimes you never want to show an empty bar.
    i=1;                // Far example, leaving a red bar at the end of a bettery's charge.
  } else {
    i=0;
  }
  switch(mDir) {
    case upBar  :
    case dwnBar : mBarMapper.setValues(inStart,inEnd,i,height); break;
    case rBar   :
    case lBar   : mBarMapper.setValues(inStart,inEnd,i,width);  break;
  }
}


void barGraphBar::setColors(colorObj* barColor,colorObj* backColor) {

    mBackColor.setColor(backColor);
    mBarColor.setColor(barColor);
    needRefresh = true;
}


void barGraphBar::setValue(float inVal) {

  if (inVal!=mBarVal) {
    mBarVal = inVal;
    mMappedVal = mBarMapper.Map(mBarVal);
    needRefresh = true;
  }
}

rect barGraphBar::calculateRect(void) {

  rect  aRect;
  int   mapPixles;

  mapPixles = round(mMappedVal);
  switch(mDir) {
    case upBar  : aRect.setAll(x,y+(height-mapPixles),width,mapPixles);  break;
    case dwnBar : aRect.setAll(x,y,width,mapPixles);                     break;
    case rBar   : aRect.setAll(x,y,mapPixles,width);                     break;
    case lBar   : aRect.setAll(x+(width-mapPixles),y,mapPixles,height);  break;
  }
  return aRect;
}


void barGraphBar::eraseRect(void) {  screen->fillRect(x,y,width,height,&mBackColor); }


void barGraphBar::drawSelf(void) {

  rect aRect;

  aRect = calculateRect();
  eraseRect();
  screen->fillRect(aRect.x,aRect.y,aRect.width,aRect.height,&mBackColor);
}

         

#define BATT_HEIGHT 14
#define BATT_WIDTH  8
#define BATT_PIN_HEIGHT 2
#define BATT_PIN_WIDTH  2

 
battPercent::battPercent(int inLocX,int inLocY)
  : drawObj(inLocX,inLocY,BATT_WIDTH,BATT_HEIGHT) {

  colorObj  backColor;
  
  mBar = new barGraphBar(x+1,y+(BATT_PIN_HEIGHT)+1,width-2,height-(BATT_PIN_HEIGHT+2));
  if (mBar) {
    mBar->setLimits(0,100);
    backColor = ourOS.getColor(SYS_PANEL_COLOR);
    mBar->setColors(&green,&backColor);
  }
  mColorMapper.addColor(100,&green);
  mColorMapper.addColor(40,&green);
  mColorMapper.addColor(25,&yellow);
  mColorMapper.addColor(15,&yellow);
  mColorMapper.addColor(10,&red);
  mColorMapper.addColor(0,&red);
  needRefresh = true;
}


battPercent::~battPercent(void) { if (mBar) delete(mBar); }


void battPercent::setPercent(int inPercent) {

  colorObj  barColor;
  colorObj  backColor;
  
  if (mBar) {
    mBar->setValue(inPercent);
    barColor = mColorMapper.Map(inPercent);
    backColor = ourOS.getColor(SYS_PANEL_COLOR);
    //mBar->setColors(&barColor,&backColor);
    mBar->setColors(&green,&backColor);
  } 
  needRefresh = true; 
}


void battPercent::drawSelf(void) {
  
  screen->drawRect(x,y+BATT_PIN_HEIGHT,BATT_WIDTH,BATT_HEIGHT-BATT_PIN_HEIGHT,&black);          // The can.
  screen->drawRect(x+((BATT_WIDTH-BATT_PIN_WIDTH)/2),y,BATT_PIN_WIDTH,BATT_PIN_HEIGHT,&black);  // The + pin.
  mBar->draw();
}
