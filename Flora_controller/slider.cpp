#include "slider.h"

slider::slider(int x,int y,int width,int height) 
  : drawObj(x,y,width,height,dragEvents) {

  setup(8,3,0,100);               // Just some nice defaults to get going.
  mBackColor.setColor(LC_BLACK);
  mOutlineColor.setColor(LC_GREY);
  mKnobColor.setColor(LC_RED);
  mInHit = false;
}


slider::~slider(void) {  }


// A bunch of stuff needs calculating all in order. Toss all the values in here.
void slider::setup(int knobWidth,int dragLineHeight,float leftVal,float rightVal) {

  int adjY;
  
  mKnob.setSize(knobWidth,height);                                // Set the size of the control knob rect.
  
  adjY = (y+(height/2))-(dragLineHeight/2);                       // Top of dragline rect.
  mDragLine.setRect(x,adjY,width,dragLineHeight);                 // Setup dragline rect.
  
  mLeftLimit = x+(mKnob.width/2);                                 // x limit takeing slider width into account.
  mRightLimit = (x+width)-(mKnob.width/2);                        // The other x limit.
  mPos2ValMap.setValues(mLeftLimit,mRightLimit,leftVal,rightVal); // Mapper for getting values.
  mVal2PosMap.setValues(leftVal,rightVal,mLeftLimit,mRightLimit); // Mapper for getting x locations.

  mLoc = x + width/2;                                             // Default to mid scale.
  setKnob();                                                      // Set control knob to mLoc.
  mNewLoc = -1;                                                  // No next location set, yet.
}

void slider::setKnob(void) {

   if (mLoc<mLeftLimit) mLoc = mLeftLimit;    // Limit the values.
   if (mLoc>mRightLimit) mLoc = mRightLimit;
   mKnob.setLocation(mLoc-(mKnob.width/2),y);
}


void slider::drawSelf(void) {

  screen->fillRect(&mKnob,&mBackColor);
  screen->fillRect(&mDragLine,&mBackColor);
  if (mNewLoc>0) {
    mLoc = mNewLoc;
    setKnob();
    mNewLoc = -1;
  }
  screen->drawRect(&mDragLine,&mOutlineColor);
  screen->fillRoundRect(&mKnob,2,&mKnobColor);
  screen->drawRoundRect(&mKnob,2,&mOutlineColor);
  //delay(10);
}


//  Special for them that drag around.
void slider::doAction(event* inEvent,point* locaPt) {

  rect  hitRect;
  
  switch(inEvent->mType) {
    case dragBegin  :
      hitRect.setRect(&mKnob);
      hitRect.insetRect(-5);
      mInHit = hitRect.inRect(locaPt);
    break;
    case dragOn     :
      if (mInHit) {
        mNewLoc = inEvent->mLastPos.x;
        setNeedRefresh();
      }
    break;
    case liftEvent  :
      if (mInHit) {
        mInHit = false;
        setNeedRefresh();
      }
    break;
    default : break;
  }
}


// Just in case you were wondering.
float slider::getValue(void) { return mPos2ValMap.Map(mLoc); }


// Great for pre-loading.
void slider::setValue(float value) {

     mNewLoc =  mVal2PosMap.Map(value);
     setNeedRefresh();               
}
