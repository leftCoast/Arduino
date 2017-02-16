#ifndef ballObj_h
#define ballObj_h

#include "movingObj.h"
#include <label.h>

#define BALL_X    61
#define BALL_Y    60
#define BALL_DIA  4
#define BALL_FPM  4   // Frames / move
#define BALL_DY   -2
#define BALL_DX   0

#define COMPASS_POINTS  18

enum courseChange { clockwise, counterClockwise };

class gameCompass {

  public:
    gameCompass(void);

    point adjustCourse(int dy,int dx,courseChange change);
  
    point courses[COMPASS_POINTS];
};


class ballObj : public movingObj {

  public:
    ballObj(movingObj* inPaddle,label* inDebug);

          void  reset(void);
  virtual void setLocation(int inX, int inY);
  virtual void drawSelf(void);
          void ballFrame(void);
    
    int   deltaY;       // Our vector..
    int   deltaX;
    int   framePerPix;
    
    int   yCount;       // How we're doing onlong that vector..
    int   xCount;
    int   frameCount;

    boolean ballLost;

    movingObj*  thePaddle;
    label*      mDebug;
};

#endif
