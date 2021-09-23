#ifndef paddleObj_h
#define paddleObj_h

#include <drawObj.h>
#include "backColorObj.h"

class paddleObj : public drawObj,
						public backColorObj {

  public:
          paddleObj(int x,int y,int width,int height);
  virtual ~paddleObj(void);

  virtual void eraseSelf(void);
  virtual void drawSelf(void);
          int  middleX(void);
};

#endif
