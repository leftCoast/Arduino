#ifndef slider_h
#define slider_h

#include <mapper.h>
#include <drawObj.h>
#include <eventMgr.h>

class slider :  public drawObj {

  public:
                slider(int x,int y,int width,int height);
  virtual       ~slider(void);

  virtual void  setup(int knobWidth,int dragLineHeight,float leftVal,float rightVal);
  virtual void  setKnob(void);
  virtual void  drawSelf(void);
  virtual void  doAction(event* inEvent,point* locaPt);   //  Special for them that drag around.
  virtual float getValue(void);                           // Just in case you were wondering.
  virtual void  setValue(float value);                    // Great for pre-loading.

          int       mLoc;
          int       mNewLoc;
          bool      mInHit;
          rect      mKnob;
          rect      mDragLine;
          mapper    mPos2ValMap;
          mapper    mVal2PosMap;
          int       mLeftLimit;
          int       mRightLimit;
          colorObj  mBackColor;
          colorObj  mOutlineColor;
          colorObj  mKnobColor;
 };

 
#endif
