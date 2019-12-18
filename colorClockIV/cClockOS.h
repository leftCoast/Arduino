#ifndef cClockOS_h
#define cClockOS_h



#define OLED_CS       10
#define OLED_RST      6
#define POT_BTN       4

#include "Wire.h"
#include "RTClib.h"
#include "adafruit_1431_Obj.h"
#include "mechButton.h"
#include "runningAvg.h"
#include "lilParser.h"
#include "fontLabel.h"
#include "litlOS.h"
#include "colorRect.h"
#include "multiMap.h"

enum  apps { homeApp = HOME_PANEL_ID, colorEditApp };



// *****************************************************
//                      litleClk
// *****************************************************


class litleClk :  public drawGroup {

  public:
          litleClk::litleClk(int x,int y);
  virtual ~litleClk(void);

  virtual void  drawSelf(void);
          void  setTime(int hour,int minute);
          void  setTColor(colorObj* aColor);
          void  setDisplay(bool trueFalse);
          bool  getDisplay(void);
  private:
          colorObj    mBackColor;
          label*      mText;
          bool        mShowing;
};



// *****************************************************
//                      nWindow
// *****************************************************


// Given a +/- range of a base value, pass back if a given
// value falls into this range. (inclusive)
class nWindow {

  public:
          nWindow(float range, float baseVal=0);
  virtual ~nWindow(void);

          void  setBase(float baseVal);
          bool  inRange(float val);

          float mRange;
          float mMin;
          float mMax;
};



// *****************************************************
//                      clockRect
// *****************************************************


class clockRect : public colorRect {

  public:
          clockRect(rect* inRect,colorObj* inColor,int inset=0);
          clockRect(int inLocX,int inLocY,int inWidth,int inHeight,int inset=0);
  virtual ~clockRect(void);

  virtual void  setPercent(float percent);
  virtual void  drawSelf(void);

          float   mPercent;
};


          
// *****************************************************
//                      homeClkPanel
// *****************************************************


class homeClkPanel : public homePanel {

  public:
          enum        mainComs { noCommand, help, setHour, setMin, setSec, reset, cEdit };
          
          homeClkPanel(void);
  virtual ~homeClkPanel(void);
  
  virtual void        setup(void);
  virtual void        loop(void);
  virtual void        drawSelf(void);
          void        checkDimmer(void);
          void        checkScreen(void);
          void        doSetHour(void);
          void        doSetMin(void);
          void        doSetSec(void);
          void        doReset(void);
          void        checkParse(void);
                
          clockRect*  mBacground;
          litleClk*   mLittleClk;
          lilParser   mParser;
          fontLabel*  shade;
          multiMap*   dimmer;
          runningAvg* smoother;
          int         dimPercent;
          DateTime    drawtime;
          bool        showTime;
          nWindow*    wind;
};



class cClockOS : public litlOS {

  public:
          cClockOS(void);
  virtual ~cClockOS(void);

  virtual int     begin(void);
          void    readParams(void);
          void    saveParams(void);
  virtual panel*  createPanel(int panelID);
};


extern RTC_DS3231   rtc;
extern mechButton   ourClicker;
extern cClockOS     ourOS;
extern boolean      haveClock;
extern colorObj     colors[];

#endif
