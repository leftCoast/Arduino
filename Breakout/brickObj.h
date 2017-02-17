#ifndef brickObj_h
#define brickObj_h

#include "movingObj.h"

#define BRICK_W       14
#define BRICK_H       6
#define NUM_BRICKS    126/BRICK_W
#define BOTTOM_BRICK  48
#define BRICK_ROWS    6
#define TOTAL_BRICKS  NUM_BRICKS*BRICK_ROWS
#define HILIGHT_PRCT  40
#define SHADOW_PRCT   60

class brickObj : public movingObj {

  public:
    brickObj(int inX,int inY);
    
            void  setColor(colorObj* inColor);
            void  hit(void);
            void  reset(void);
    virtual void  drawSelf(void);
    
    colorObj  brickColor;
    colorObj  hilight;
    colorObj  shadow;
    int       homeX;
    int       homeY;
};

extern brickObj* brickList[];
extern void resetBricks(void);
extern boolean bricks(void);

#endif
