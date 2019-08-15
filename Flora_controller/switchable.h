#ifndef switchable_h
#define switchable_h

#include <drawObj.h>

class switchable : public drawObj {
  
  public :
                  switchable();
                  switchable(rect* inRect,eventSet inEventSet=noEvents);
                  switchable(int inLocX, int inLocY, int inWidth,int inHeight,eventSet inEventSet=noEvents);
  virtual         ~switchable(void);

  virtual void    select(bool onOff);

          eventSet  mSavedSet;
          bool      mOnOff;
};

#endif
