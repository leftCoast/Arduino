#ifndef UI_h
#define UI_h

#include <label.h>
#include <flasher.h>
#include "dataLog.h"
#include "globals.h"

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
};


class UI  : public dataLog,
            public idler,
            public timeObj {

  public:
                UI(void);
  virtual       ~UI(void);

  virtual void  begin(void);
  virtual void  idle(void);
          void  sensorDeath(void);
          
          bool        mHaveScreen;
          percView*   mLimit;
          percView*   mMoisture;
          label*      mSlash;
          label*      mKey;
          stateView*  mState;
          flasher*    mLoggingLED;
          float       mLastMoist;
          int         mLastLimit;
          weDo        mLastState;
};


extern UI ourDisplay;

#endif
