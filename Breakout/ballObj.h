#ifndef ballObj_h
#define ballObj_h

#include "movingObj.h"
#include <label.h>

#define BALL_X    61
#define BALL_Y    50
#define BALL_DIA  6
#define BALL_FPP  4   // Frames / pixel
#define BALL_DY   2
#define BALL_DX   1

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
          void  chageDirection(courseChange change);
  virtual void setLocation(int inX, int inY);
  virtual void drawSelf(void);
          void  ballFrame(void);
    
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
