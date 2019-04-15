#include "icons.h"
#include "cellOS.h"


// *********************************************
// **************** SMSMark ********************
// *********************************************

#define SMS_X0  0
#define SMS_X1  10
#define SMS_X2  40
#define SMS_X3  50
#define SMS_X4  60
#define SMS_X5  90
#define SMS_X6  100

#define SMS_Y0  0
#define SMS_Y1  25
#define SMS_Y2  40
#define SMS_Y3  50
#define SMS_Y4  80
#define SMS_Y5  100


SMSMark::SMSMark(rect* aRect)
  : stampObj(aRect) {

  mColor.setColor(LC_GREY); // Default color.
  mapper  xMapper(0,100,x,x+width);
  mapper  yMapper(0,100,y,y+height);
  mPoint1.x = x;                          mPoint1.y = round(yMapper.Map(SMS_Y1));
  mPoint2.x = round(xMapper.Map(SMS_X3)); mPoint2.y = round(yMapper.Map(SMS_Y2));
  mPoint3.x = x+width;                    mPoint3.y = round(yMapper.Map(SMS_Y1));
  mPoint4.x = round(xMapper.Map(SMS_X1)); mPoint4.y = round(yMapper.Map(SMS_Y4));
  mPoint5.x = round(xMapper.Map(SMS_X2)); mPoint5.y = round(yMapper.Map(SMS_Y3));
  mPoint6.x = round(xMapper.Map(SMS_X4)); mPoint6.y = round(yMapper.Map(SMS_Y3));
  mPoint7.x = round(xMapper.Map(SMS_X5)); mPoint6.y = round(yMapper.Map(SMS_Y4));
}


SMSMark::~SMSMark(void) {  }

void SMSMark::setColor(colorObj* inColor) { mColor.setColor(inColor); }


void SMSMark::stamp(void) {

  screen->drawRect(this,&mColor);
  screen->drawLine(&mPoint1,&mPoint2,&mColor);
  screen->drawLine(&mPoint2,&mPoint3,&mColor);
  screen->drawLine(&mPoint4,&mPoint5,&mColor);
  screen->drawLine(&mPoint6,&mPoint7,&mColor);
}



// *********************************************
// ************** barGraphBar ******************
// *********************************************


#define NO_VALUE  -1

barGraphBar::barGraphBar(int inX,int inY,int inwidth,int inHeight,barGraphDir inDir,bool inBinary,eventSet inEventSet)
  : drawObj(inX,inY,inwidth,inHeight,inEventSet) {

  mDir = inDir;         // Set Direction.
  mBarVal = NO_VALUE;   // Flag that we have no value.
  mBinary = inBinary;   // Sometimes is all or nothin'
  mLeavePixel = false;
  needRefresh = false;  // Why?! Because we are not ready to be drawn yet.
}


barGraphBar::~barGraphBar(void) {  }


// This is for binary on/off bars. Below cutoff, no bar, cutoff and above, full bar.
void barGraphBar::setCutoff(float inCutoff,bool leaveAPixle) {

  mCutoff = inCutoff;
  mLeavePixel = leaveAPixle;
}


void barGraphBar::setLimits(float inStart,float inEnd,bool leaveAPixle) {

  int i;

  mLeavePixel = leaveAPixle;
  if (mBinary) {
    setCutoff((inStart+inEnd)/2,leaveAPixle);
  } else {
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
    case upBar  : aRect.setRect(x,y+(height-mapPixles),width,mapPixles);  break;
    case dwnBar : aRect.setRect(x,y,width,mapPixles);                     break;
    case rBar   : aRect.setRect(x,y,mapPixles,height);                    break;
    case lBar   : aRect.setRect(x+(width-mapPixles),y,mapPixles,height);  break;
  }
  return aRect;
}


void barGraphBar::eraseRect(void) {  screen->fillRect(x,y,width,height,&mBackColor); }


void barGraphBar::drawSelf(void) {

  rect aRect;
  
  if (mBinary) {
    eraseRect();
    if (mBarVal>=mCutoff) {
      screen->fillRect(x,y,width,height,&mBarColor);
    } else if (mLeavePixel) {
      switch(mDir) {
        case upBar  : screen->drawHLine(x,y+height-1,width,&mBarColor);   break;
        case dwnBar : screen->drawHLine(x,y,width,&mBarColor);          break;
        case rBar   : screen->drawVLine(x,y,height,&mBarColor);         break;
        case lBar   : screen->drawVLine(x+width-1,y,height,&mBarColor);   break;
      }
    }
  } else if (mBarVal!=NO_VALUE) {
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
  
  mBar = new barGraphBar(1,BATT_PIN_HEIGHT+1,width-2,height-(BATT_PIN_HEIGHT+2));
  if (mBar) {
    addObj(mBar);
    mBar->setLimits(0,100);
    mBar->setColors(&menuBarColor,&menuBarColor);       // Forces an empty bar if nothing's been set.
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


void battPercent::setPercent(int inPercent,colorObj* backColor) {

  colorObj  barColor;
  
  if (mBar) {
    mBar->setValue(inPercent);
    barColor = mColorMapper.Map(inPercent);
    mBar->setColors(&barColor,backColor);
  } 
  needRefresh = true; 
}


void battPercent::drawSelf(void) {

  screen->drawRect(x,y+BATT_PIN_HEIGHT,BATT_WIDTH,BATT_HEIGHT-BATT_PIN_HEIGHT,&battLineColor);          // The can.
  screen->drawRect(x+((BATT_WIDTH-BATT_PIN_WIDTH)/2),y,BATT_PIN_WIDTH,BATT_PIN_HEIGHT,&battLineColor);  // The + pin.
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
    mBars[i] = new barGraphBar(i*(RSSI_BAR_WID+RSSI_BAR_SPACE)+1,(height-barHeight)-1,RSSI_BAR_WID,barHeight,upBar,true);
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
      mBars[i]->setColors(&barColor,&battLineColor);
    } 
  }
  needRefresh = true; 
}


void RSSIicon::drawSelf(void) { }
