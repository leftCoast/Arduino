#include "slider.h"

slider::slider(int x,int y,int width,int height) 
  : drawObj(x,y,width,height,dragEvents) {

  setup(DEF_SLIDER_KNOB_W,DEF_SLIDER_LINE_H,DEF_SLIDER_LVAL,DEF_SLIDER_RVAL); // Just some nice defaults to get going.
  mBackColor.setColor(DEF_SLIDER_BCOLOR);
  mOutlineColor.setColor(DEF_SLIDER_OLCOLOR);
  mKnobColor.setColor(LC_RED);
  mInHit = false;
  mBeenReset = false;
}


slider::~slider(void) {  }


// A bunch of stuff needs calculating all in order. Toss all the values in here.
void slider::setup(int knobWidth,int dragLineHeight,float leftVal,float rightVal) {

  int adjY;
  
  mKnob.setSize(knobWidth,height);                                // Set the size of the control knob rect.
  
  adjY = (y+(height/2))-(dragLineHeight/2);                       // Top of dragline rect.
  mDragLine.setRect(x,adjY,width,dragLineHeight);                 // Setup dragline rect.
  
  mLeftLimit = x+(mKnob.width/2);                                 // x limit taking slider width into account.
  mRightLimit = (x+width)-(mKnob.width/2);                        // The other x limit.
  mPos2ValMap.setValues(mLeftLimit,mRightLimit,leftVal,rightVal); // Mapper for getting values.
  mVal2PosMap.setValues(leftVal,rightVal,mLeftLimit,mRightLimit); // Mapper for getting x locations.

  mLoc = x + width/2;                                             // Default to mid scale.
  setKnob();                                                      // Set control knob to mLoc.
  mNewLoc = -1;                                                   // No next location set, yet.
  mBeenReset = true;                                              // Everything has changed, better clear all drawing.
  setNeedRefresh();
}

// Internally mLoc is set to a pixel value. This moves the mKnob rect
// to reflect that. Does some limiting too so other's don't have to.
void slider::setKnob(void) {

   if (mLoc<mLeftLimit) mLoc = mLeftLimit;    // Limit the values.
   if (mLoc>mRightLimit) mLoc = mRightLimit;
   mKnob.setLocation(mLoc-(mKnob.width/2),y); // Move mKnob to its new location taking its width into account.
}


// I was in a hurry and encorperated the erase with drawSelf().
// Maybe a bad idea, but for now its working.
void slider::drawSelf(void) {

  if (mBeenReset) {                                 // If the reset flag is set..
      screen->fillRect(this,&mBackColor);           // It means we have no idea how things were left, so we clear everything.
      mBeenReset = false;                           // Clear the flag.
  }
  if (mNewLoc>0) {                                  // If we have a new location..
    screen->fillRect(&mKnob,&mBackColor);           // Blank out the knob.
    mLoc = mNewLoc;                                 // Update our location.
    setKnob();                                      // Update the knob rect..
    mNewLoc = -1;                                   // Set flag that there is no more new location.
  }
  screen->drawRect(&mDragLine,&mOutlineColor);      // Draw the drag line.
  screen->fillRoundRect(&mKnob,2,&mKnobColor);      // Fill in the knob.
  screen->drawRoundRect(&mKnob,2,&mOutlineColor);   // Finish drawing the knob.
}


// Odd deal here. The object is the entire control. But what we want to control is just the knob
// itself. And, there's issues with that as well. Then knob is small and the screen is, well, not
// terribly accurate. So we actually make up our own slighty bigger (invisable) hit rect in here
// and let Mr finger aim at that.
void slider::doAction(event* inEvent,point* locaPt) {

  rect  hitRect;
  
  switch(inEvent->mType) {              // We have an incoming event. Check the type.
    case dragBegin  :                   // A starting off a drag action.  
      hitRect.setRect(&mKnob);          // Make a hitRect the size and position of the knob.
      hitRect.insetRect(-6);            // Expand it by 6 pixels.
      mInHit = hitRect.inRect(locaPt);  // Now we use this to make sure the hit was where we specified.
    break;                              // That's all we wanted to do at this point. Lets Jet!
    case dragOn     :                   // Drag on event.
      if (mInHit) {                     // If this was started in our reduced hit rect..
        mNewLoc = inEvent->mLastPos.x;  // Grab the last location, save it into new location.
        setNeedRefresh();               // Set refresh and the drawSelf() method with handle the rest.
      }
    break;                              // Done here.
    case liftEvent  :                   // Drag is over. The finger has lifted!
      if (mInHit) {                     // Were we paying attenttion to it at all?
        mInHit = false;                 // If we were we're done now.
        setNeedRefresh();               // Last drawing.
      }
    break;                              // All done.
    default : break;                    // Shut up compiler!
  }
}


// Just in case you were wondering.
float slider::getValue(void) { return mPos2ValMap.Map(mLoc); }


// Great for pre-loading.
void slider::setValue(float value) {

     mNewLoc =  mVal2PosMap.Map(value);
     setNeedRefresh();               
}
