#include "icons.h"
#include "litlOS.h"



// *********************************************
// ************** barGraphBar ******************
// *********************************************


#define NO_VALUE  -1

barGraphBar::barGraphBar(int inX,int inY,int inwidth,int inHeight,barGraphDir inDir,bool inTouch)
  : drawObj(inX,inY,inwidth,inHeight,inTouch) {

  mDir = inDir;         // Set Direction.
  mBarVal = NO_VALUE;   // Flag that we have no value.
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
    case rBar   : aRect.setAll(x,y,mapPixles,height);                    break;
    case lBar   : aRect.setAll(x+(width-mapPixles),y,mapPixles,height);  break;
  }
  return aRect;
}


void barGraphBar::eraseRect(void) {  screen->fillRect(x,y,width,height,&mBackColor); }


void barGraphBar::drawSelf(void) {

  rect aRect;
  
  if (mBarVal!=NO_VALUE) {
    aRect = calculateRect();
    eraseRect();
    screen->fillRect(aRect.x,aRect.y,aRect.width,aRect.height,&mBarColor);
  }
}


         
// *********************************************
// ************** battPercent ******************
// *********************************************


#define BATT_HEIGHT 14
#define BATT_WIDTH  8
#define BATT_PIN_HEIGHT 2
#define BATT_PIN_WIDTH  2

 
battPercent::battPercent(int inLocX,int inLocY)
  : drawGroup(inLocX,inLocY,BATT_WIDTH,BATT_HEIGHT) {

  colorObj  backColor(SYS_PANEL_COLOR);
  
  mBar = new barGraphBar(1,BATT_PIN_HEIGHT+1,width-2,height-(BATT_PIN_HEIGHT+2));
  if (mBar) {
    addObj(mBar);
    mBar->setLimits(0,100);
    mBar->setColors(&backColor,&backColor);       // Forces an empty bar if nothing's been set.
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
  colorObj  backColor(SYS_PANEL_COLOR);
  
  if (mBar) {
    mBar->setValue(inPercent);
    barColor = mColorMapper.Map(inPercent);
    mBar->setColors(&barColor,&backColor);
  } 
  needRefresh = true; 
}


void battPercent::drawSelf(void) {
  
  screen->drawRect(x,y+BATT_PIN_HEIGHT,BATT_WIDTH,BATT_HEIGHT-BATT_PIN_HEIGHT,&black);          // The can.
  screen->drawRect(x+((BATT_WIDTH-BATT_PIN_WIDTH)/2),y,BATT_PIN_WIDTH,BATT_PIN_HEIGHT,&black);  // The + pin.
}



// *********************************************
// **************** RSSIicon *******************
// *********************************************


#define RSSI_HEIGHT     15
#define RSSI_BAR_WID    3
#define RSSI_BAR_SPACE  1


RSSIicon::RSSIicon(int inLocX,int inLocY)
  : drawGroup(inLocX,inLocY,RSSI_NUM_BARS*(RSSI_BAR_WID+RSSI_BAR_SPACE)+2,RSSI_HEIGHT) {

  colorObj  backColor(LC_BLACK);
  int       startPt;
  int       limitPt;
  int       barHeight;
  
  mapper barHMapper(0,RSSI_NUM_BARS-1,2,RSSI_HEIGHT-2);
  mapper barLimitMapper(0,RSSI_NUM_BARS-1,1,31);
  startPt = 0; 
  for (int i=0;i<RSSI_NUM_BARS;i++) {
    barHeight = round(barHMapper.Map(i));
    mBars[i] = new barGraphBar(i*(RSSI_BAR_WID+RSSI_BAR_SPACE)+1,(height-barHeight)-1,RSSI_BAR_WID,barHeight,rBar);
    if (mBars[i]) {
      limitPt = round(barLimitMapper.Map(i));
      mBars[i]->setLimits(startPt,limitPt,i==0);
      startPt = limitPt;
      mBars[i]->setColors(&backColor,&backColor);       // Forces an empty bar if nothing's been set.
      addObj(mBars[i]);
    }
  }
  mColorMapper.addColor(31,&green);
  mColorMapper.addColor(13,&green);
  mColorMapper.addColor(12,&yellow);
  mColorMapper.addColor(10,&yellow);
  mColorMapper.addColor(8,&red);
  mColorMapper.addColor(0,&red);
  needRefresh = true;
}


RSSIicon::~RSSIicon(void) {

  for (int i=0;i<RSSI_NUM_BARS;i++) {
    if (mBars[i]) delete(mBars[i]);
  }
}


void RSSIicon::setRSSI(int inRSSI) {

  colorObj  barColor;
  colorObj  backColor(LC_CHARCOAL);

  barColor = mColorMapper.Map(inRSSI);

  for (int i=0;i<RSSI_NUM_BARS;i++) {
    if (mBars[i]) {
      mBars[i]->setValue(inRSSI);
      mBars[i]->setColors(&barColor,&backColor);
    } 
  }
  needRefresh = true; 
}


void RSSIicon::drawSelf(void) {

  colorObj  aColor(LC_GREY);
  screen->drawRect(x,y,width,height,&aColor);
  screen->fillRect(x-1,y-1,width-1,height-1,&black);
  //screen->drawLine(x,y+height-1,x+width-1,y+height-1,&aColor);
  //screen->drawLine(x+width-1,y+height-1,x+width-1,y,&aColor);    <- two point vertical seems to not work.   
}



// *********************************************
// ************* closeBtn *****************
// *********************************************


#define CLOSE_X 2
#define CLOSE_Y 2
#define CLOSE_W 14
#define CLOSE_H 14

closeBtn::closeBtn(void)
  : drawObj(CLOSE_X,CLOSE_Y,CLOSE_W,CLOSE_H,true) { }

  
closeBtn::~closeBtn(void) {  }

void closeBtn::drawSelf(void) {
  colorObj  darkRed(LC_RED);
  darkRed.blend(&black,50);

  if (clicked) {
    screen->fillRoundRect(x,y,width,height,3,&white);
    screen->drawRoundRect(x,y,width,height,3,&red);
    screen->drawCircle(x+3,y+2,9,&white);
  } else {
    screen->fillRoundRect(x,y,width,height,3,&red);
    screen->drawRoundRect(x,y,width,height,3,&darkRed);
    screen->drawCircle(x+3,y+2,9,&darkRed);
  }
}