#ifndef ballObj_h
#define ballObj_h

#include "movingObj.h"
#include "breakout.h"


#define COMPASS_POINTS  18

enum courseChange { clockwise, counterClockwise };

class gameCompass {

  public:
          gameCompass(void);
  virtual ~gameCompass(void);

    point adjustCourse(int dy,int dx,courseChange change);
  
    point courses[COMPASS_POINTS];
};


class ballObj : public movingObj {

  public:
          ballObj(movingObj* inPaddle);
  virtual ~ballObj(void);

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
};

#endif
