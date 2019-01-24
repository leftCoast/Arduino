#ifndef enterArrow_h
#define enterArrow_h

#include <stampObj.h>


class enterArrow : public stampObj {

  public:
          enterArrow(rect* inRect);
  virtual ~enterArrow(void);
          
          void setColors(colorObj* foreColor,colorObj* backColor);
  virtual void stamp(void);

          colorObj  mForeColor;
          colorObj  mBackColor;
};

#endif