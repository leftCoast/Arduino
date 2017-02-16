#ifndef brickObj_h
#define brickObj_h

#include "movingObj.h"

#define BRICK_W       15
#define BRICK_H       8
#define NUM_BRICKS    128/BRICK_W
#define BOTTOM_BRICK  48
#define BRICK_ROWS    5
#define TOTAL_BRICKS  NUM_BRICKS*BRICK_ROWS

class brickObj : public movingObj {

  public:
    brickObj(int inX,int inY);
    
            void  setColor(colorObj* inColor);
            void  hit(void);
            void  reset(void);
    virtual void  drawSelf(void);
    
    colorObj brickColor;
    int      homeX;
    int      homeY;
};

extern brickObj* brickList[];

#endif
