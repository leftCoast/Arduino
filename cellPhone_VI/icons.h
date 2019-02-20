#ifndef icons_h
#define icons_h

#include "drawObj.h"



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


/*
// *********************************************
// ************* closeBtn *****************
// *********************************************

class closeBtn : public drawObj {

  public:
            closeBtn(void);
    virtual ~closeBtn(void);

    virtual void drawSelf(void);
};

*/

#endif
