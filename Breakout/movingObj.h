#ifndef movingObj_h
#define movingObj_h

#include <drawObj.h>

class movingObj : public drawObj {

  public:
    movingObj(int inX, int inY, word inWidth,word inHeight);

          void setBackColor(colorObj* inBackColor);
  virtual void setLocation(int inX, int inY);
  virtual void eraseSelf(void);
  virtual void drawSelf(void);

          colorObj  backColor;
          int       oldX;
          int       oldY;
};

#endif

