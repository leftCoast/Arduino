#ifndef cClockOS_h
#define cClockOS_h



#define OLED_CS       10
#define OLED_RST      6
#define POT_BTN       4
#define LABEL_OFFSET  1
#define PERC_WINDOW   3

#include "Wire.h"
#include "RTClib.h"
#include "adafruit_1431_Obj.h"
#include "mechButton.h"
#include "runningAvg.h"
#include "lilParser.h"
#include "fontLabel.h"
#include "litlOS.h"
#include "colorRect.h"


enum  apps { homeApp = HOME_PANEL_ID, colorEditApp };

// *****************************************************
//                      randCRect
// *****************************************************


class randCRect : public colorRect {

  public:
          randCRect(rect* inRect,colorObj* inColor,int inset=0);
          randCRect(int inLocX,int inLocY,int inWidth,int inHeight,int inset=0);
  virtual ~randCRect(void);

  virtual void  setPercent(int percent);
  virtual void  drawSelf(void);

          int   mPercent;
};


          
// *****************************************************
//                      homeClkPanel
// *****************************************************


class homeClkPanel : public homePanel {

  public:
          enum        mainComs { noCommand, help, setHour, setMin, setSec, see };
          
          homeClkPanel(void);
  virtual ~homeClkPanel(void);
  
  virtual void        setup(void);
  virtual void        loop(void);
  virtual void        drawSelf(void);
          void        drawClock(void);
          void        checkScreen(void);
          void        doSetHour(void);
          void        doSetMin(void);
          void        doSetSec(void);
          void        doSee(void);
          void        checkParse(void);
                
          randCRect*  mBacground;
          colorRect*  mTBackground;
          lilParser   mParser;
          colorObj    colors[12];
          fontLabel*  num;
          fontLabel*  shade;
          label*      timeDisp;
          mapper*     dimmer;
          runningAvg* smoother;
          int         dimPercent;
          DateTime    drawtime;
          bool        showTime;

};



class cClockOS : public litlOS {

  public:
          cClockOS(void);
  virtual ~cClockOS(void);

  virtual int     begin(void);
  virtual panel*  createPanel(int panelID);
};


extern RTC_DS3231   rtc;
extern mechButton   ourClicker;
extern cClockOS     ourOS;
extern boolean      haveClock;

#endif
