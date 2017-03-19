#ifndef liveText_h
#define liveText_h

#include <timeObj.h>
#include <idlers.h>
#include <colorObj.h>
#include <label.h>


class liveText : public label,
                 public idler, 
                 public colorMultiMap,
                 public timeObj {

  public:
                liveText(int x,int y, word width, word height,int framerateMs,bool loop=false);
                ~liveText(void);
                
  virtual boolean wantRefresh(void);
          void    addAColor(int timeMs,colorObj* color);
          void    hold(void);
          void    release(bool reset=true);
  virtual void    idle(void);
  virtual void    drawSelf(void);
          void    setCalcColor(void);
  
          bool  loop;
          int   maxTime;
          int   frame;
          bool  holding;
};

#endif
