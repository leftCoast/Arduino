#ifndef icons_h
#define icons_h

#include "drawObj.h"
#include <stampObj.h>

// *********************************************
// **************** SMSMark ********************
// *********************************************


class SMSMark : public stampObj {

public:
          SMSMark(rect* aRect);
  virtual ~SMSMark(void);

          void setColor(colorObj* inColor);
  virtual void stamp(void);

          colorObj  mColor;
          point     mPoint1;
          point     mPoint2;
          point     mPoint3;
          point     mPoint4;
          point     mPoint5;
          point     mPoint6;
          point     mPoint7;
};



// *********************************************
// ************** barGraphBar ******************
// *********************************************


enum barGraphDir { upBar, dwnBar, rBar, lBar };

class barGraphBar : public drawObj {

  public:
            barGraphBar(int inX,int inY,int inwidth,int inHeight,barGraphDir inDir=upBar,bool inBinary=false,eventSet inEventSet=noEvents);
    virtual ~barGraphBar(void);

    virtual void        setCutoff(float inCutoff,bool leaveAPixle=false);
    virtual void        setLimits(float inStart,float inEnd,bool leaveAPixle=true);
    virtual void        setColors(colorObj* barColor,colorObj* backColor);
    virtual void        setValue(float inVal);
    virtual rect        calculateRect(void);
    virtual void        eraseRect(void);

    virtual void        drawSelf(void);

            mapper        mBarMapper;
            barGraphDir   mDir;
            bool          mBinary;
            bool          mLeavePixel;
            float         mCutoff;
            float         mBarVal;
            float         mMappedVal;
            colorObj      mBarColor;
            colorObj      mBackColor;
};




// *********************************************
// ************** battPercent ******************
// *********************************************


class battPercent : public drawGroup {

  public:
          battPercent(int inLocX,int inLocY);
  virtual ~battPercent(void);

          void  setPercent(int inPercent,colorObj* backColor);
  virtual void  drawSelf(void);

          barGraphBar*  mBar;
          colorMultiMap mColorMapper;
};



// *********************************************
// **************** RSSIicon *******************
// *********************************************


#define RSSI_NUM_BARS   4

class RSSIicon : public drawGroup {

  public:
          RSSIicon(int inLocX,int inLocY);
  virtual ~RSSIicon(void);

          void  setRSSI(int inRSSI);
  virtual void  drawSelf(void);

          barGraphBar*    mBars[RSSI_NUM_BARS];
          colorMultiMap   mColorMapper;
};


#endif
