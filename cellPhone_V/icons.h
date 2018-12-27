#ifndef icons_h
#define icons_h

#include "drawObj.h"

enum barGraphDir { upBar, dwnBar, rBar, lBar };

class barGraphBar : public drawObj {

  public:
            barGraphBar(int inX,int inY,int inwidth,int inHeight,barGraphDir inDir=upBar,bool inTouch=false);
    virtual ~barGraphBar(void);

    virtual void        setLimits(float inStart,float inEnd,bool leaveAPixle=true);
    virtual void        setColors(colorObj* barColor,colorObj* backColor);
    virtual void        setValue(float inVal);
    virtual rect        calculateRect(void);
    virtual void        eraseRect(void);

    virtual void        drawSelf(void);

            mapper        mBarMapper;
            barGraphDir   mDir;
            float         mBarVal;
            float         mMappedVal;
            colorObj      mBarColor;
            colorObj      mBackColor;
};


class battPercent : public drawGroup {

  public:
          battPercent(int inLocX,int inLocY);
  virtual ~battPercent(void);

          void  setPercent(int inPercent);
  virtual void  drawSelf(void);

          barGraphBar*  mBar;
          colorMultiMap mColorMapper;
};


#define RSSI_HEIGHT     14
#define RSSI_NUM_BARS   6
#define RSSI_BAR_WID    3
#define RSSI_BAR_SPACE  1

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
