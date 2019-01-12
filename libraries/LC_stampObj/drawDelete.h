#ifndef drawDelete_h
#define drawDelete_h

#include <stampObj.h>


class drawDelete : public stampObj {

  public:
          drawDelete(rect* inRect);
  virtual ~drawDelete(void);
          
          void setColors(colorObj* foreColor,colorObj* backColor);
  virtual void stamp(void);

          colorObj  mForeColor;
          colorObj  mBackColor;
};

#endif