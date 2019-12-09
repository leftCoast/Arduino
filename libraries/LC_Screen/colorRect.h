#ifndef colorRect_h
#define colorRect_h

#include <drawObj.h>
#include <colorObj.h>

// A drawObject that is also a colorObject. Ever so handy! Good for backgrounds.
// It can also auto generate an inset border. If that's what your into.

class colorRect : public drawObj,
                  public colorObj {

  public:
            colorRect(void);
            colorRect(rect* inRect,colorObj* inColor,int inset=0);
            colorRect(int inLocX,int inLocY,int inWidth,int inHeight,int inset=0);
    virtual ~colorRect(void);

    virtual void  setInset(int inset);
    virtual void  drawSelf(void);
    virtual void  setColor(byte r,byte g,byte b);
    virtual void  setColor(word color16);
    virtual void  setColor(colorObj* inColor);

            int   mInset;
};

#endif
