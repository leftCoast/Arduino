#ifndef fontLabel_h
#define fontLabel_h

#include "label.h"

class fontLabel : public label {

  public:
                fontLabel(int inX, int inY, int inWidth,int inHeight);
  virtual       ~fontLabel(void);

  virtual void  drawSelf(void);
};


#endif
