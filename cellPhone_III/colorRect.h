#ifndef colorRect_h
#define colorRect_h

#include <drawObj.h>
#include <colorObj.h>

class colorRect : public drawObj,
                  public colorObj {

  public:
            colorRect(int inLocX,int inLocY,word inWidth,word inHeight,int inset=0);
    virtual ~colorRect(void);

    virtual void  drawSelf(void);
    virtual void  setColor(byte r,byte g,byte b);
    virtual  void  setColor(word color16);
    virtual void  setColor(colorObj* inColor);

            int   mInset;
};

#endif
