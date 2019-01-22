#ifndef brickObj_h
#define brickObj_h

#include  <bmpPipe.h>
#include "breakout.h"
#include "movingObj.h"

extern int  brickIndex;

class brickObj : public movingObj,
                 public bmpPipe {

  public:
          brickObj(int inX,int inY);
  virtual ~brickObj(void);
    
          void  setColor(colorObj* inColor);
          void  setColor(char* path);
          void  hit(void);
          void  reset(void);
  virtual void  drawSelf(void);
    
          int       homeX;
          int       homeY;
};

extern brickObj* brickList[];
extern void resetBricks(void);
extern boolean bricks(void);

#endif
