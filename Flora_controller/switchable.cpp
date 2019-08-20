
#include "switchable.h"

switchable::switchable() 
  : drawObj() {
  
  mSavedSet = mEventSet;
  mOnOff  = true;
}


switchable::switchable(rect* inRect,eventSet inEventSet)
  : drawObj(inRect,inEventSet) {
  
  mSavedSet = mEventSet;
  mOnOff  = true;
}


switchable::switchable(int inLocX, int inLocY, int inWidth,int inHeight,eventSet inEventSet)
   : drawObj(inLocX,inLocY,inWidth,inHeight,inEventSet) {
 
  mSavedSet = mEventSet;
  mOnOff  = true;
}


switchable::~switchable(void) { }


void switchable::select(bool onOff) {

  if (onOff) {                          // If we're turning on..
    setEventSet(mSavedSet);             // We replace our saved event set.
  } else {                              // Else, turning off..
    if (theTouched==this) {             // In the case that we are the touched..
      mEventSet = noEvents;             // We force drop the event set
    } else {                            // Else we're NOT the touched?
      setEventSet(noEvents);            // Drop the event set through normal channels.
    }
  }
  mOnOff = onOff;                       // Save off our new state.
  setNeedRefresh();                     // Call for a refresh. We may need it. 
}

           
