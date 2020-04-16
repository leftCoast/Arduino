#ifndef UI_h
#define UI_h

#include <label.h>
#include <flasher.h>
#include "dataLog.h"
#include "globals.h"
#include <liveText.h>
#include <colorObj.h>
#include <bitmap.h>

class percView : public label {

  public:
                percView(int x,int y);
  virtual       ~percView(void);

          void  setPercent(float percent);
};


class stateView : public label {

  public:
                stateView(int x,int y);
  virtual       ~stateView(void);
  
          void  setState(weDo inState);
          void  percentDone(int inPerc);
  virtual void  drawSelf(void);

          weDo      mState;
          int       mPerc;
          colorObj  mReadColor;
          colorObj  mWaterColor;
          colorObj  mSoakColor;
          bitmap    mBMap;
};


class logInd :  public liveText {

  public:
                logInd(int x,int y);
  virtual       ~logInd(void);

          void  logging(bool onOff);
};


class UI  : public dataLog,
            public idler,
            public timeObj {

  public:
                UI(void);
  virtual       ~UI(void);

  virtual void  begin(void);
          void  setColorMap(int limit);
  virtual void  idle(void);
          void  sensorDeath(void);
          
          bool            mHaveScreen;
          percView*       mLimit;
          percView*       mMoisture;
          label*          mSlash;
          label*          mKey;
          stateView*      mState;
          logInd*         mLoggingInd;
          float           mLastMoist;
          int             mLastLimit;
          weDo            mLastState;
          colorObj        mWetColor;
          colorObj        mDryColor;
          colorMultiMap*  mColorMap;
};


extern UI ourDisplay;

#endif
